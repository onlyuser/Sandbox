// REFERENCE:
// "How it's better to invoke gdb from program to print its stacktrace?"
// http://stackoverflow.com/questions/3151779/how-its-better-to-invoke-gdb-from-program-to-print-its-stacktrace/4611112#4611112

// CONCEPTS:
// * backtrace
// * line number resolution using addr2line

#include <stdio.h> // FILE
#include <stdlib.h> // free
#include <string.h> // strdup
#include <stdarg.h> // va_list
#include <iostream> // std::cout
#include <unistd.h> // getpid
#include <iomanip> // std::setfill
#include <regex.h> // regex_t
#include <string> // std::string
#include <vector> // std::vector
#include <sstream> // std::stringstream
#include <execinfo.h> // backtrace_symbols
#include <ucontext.h> // ucontext_t
#include <cxxabi.h> // abi::__cxa_demangle
#include <sys/wait.h> // waitpid
#include <bits/signum.h> // SIGSEGV

#ifdef __x86_64__
    #define REG_EIP REG_RIP
#endif

#define MAX_EXECNAME_SIZE   1024
#define MAX_PIPEBUF_SIZE    128
#define MAX_BACKTRACE_DEPTH 16
#define LIBC_START_MAIN     "__libc_start_main"

static std::string get_execname()
{
    static std::string execname;
    if(!execname.empty())
        return execname;
    char buf[MAX_EXECNAME_SIZE];
    int n = readlink("/proc/self/exe", buf, sizeof(buf)-1);
    if(n == -1)
        return "";
    buf[n] = '\0';
    execname = buf;
    return execname;
}

static std::string shell_capture(std::string cmd)
{
    FILE* file = popen(cmd.c_str(), "r");
    if(!file)
        return "";
    char buf[MAX_PIPEBUF_SIZE];
    std::string result = "";
    while(!feof(file))
    {
        if(fgets(buf, sizeof(buf), file))
            result += buf;
    }
    pclose(file);
    return result.substr(0, result.length()-1);
}

static std::string get_basename(std::string filename)
{
    std::string _basename;
    char* buf = strdup(filename.c_str());
    _basename = basename(buf);
    free(buf);
    return _basename;
}

static bool match_regex(std::string s, std::string pattern, int nmatch, ...)
{
    bool result = true;
    regex_t preg;
    if(regcomp(&preg, pattern.c_str(), REG_ICASE|REG_EXTENDED))
        return false;
    regmatch_t* pmatch = new regmatch_t[nmatch];
    if(pmatch)
    {
        int status = regexec(&preg, s.c_str(), nmatch, pmatch, 0);
        regfree(&preg);
        if(!status)
        {
            va_list ap;
            va_start(ap, nmatch);
            for(int i = 0; i<nmatch; i++)
            {
                std::string* ptr = va_arg(ap, std::string*);
                if(ptr)
                    *ptr = s.substr(pmatch[i].rm_so, pmatch[i].rm_eo-pmatch[i].rm_so);
            }
            va_end(ap);
        }
        else
            result = false;
        delete[] pmatch;
    }
    else
        result = false;
    return result;
}

static void backtrace_sighandler(int sig, siginfo_t* info, void* secret)
{
    std::cout << "stack array for " << get_execname() << " pid=" << getpid() << std::endl;
    std::cout << "Error: signal " << sig << ":" << std::endl;
    void* array[MAX_BACKTRACE_DEPTH];
    int size = backtrace(array, MAX_BACKTRACE_DEPTH);
    array[1] = reinterpret_cast<void*>(
            reinterpret_cast<ucontext_t*>(secret)->uc_mcontext.gregs[REG_EIP]);
    char** symbols = backtrace_symbols(array, size);
    for(int i = 1; i<size; i++)
    {
        std::stringstream ss;
        ss << "addr2line " << array[i] << " -e " << get_execname();
        std::string exec_basename = get_basename(shell_capture(ss.str()));
        std::string module, mangled_name, offset, address;
        if(match_regex(symbols[i], "(.*)[\(](.*)[+](.*)[)] [\[](.*)[]]", 5,
                NULL,
                &module,
                &mangled_name,
                &offset,
                &address))
        {
            if(mangled_name == LIBC_START_MAIN)
                break;
            int status;
            char* demangled_name = abi::__cxa_demangle(mangled_name.c_str(), NULL, 0, &status);
            if(status == 0)
            {
                std::cout << "#" << i
                        << "  0x" << std::setfill('0') << std::setw(16) << std::hex
                        << reinterpret_cast<size_t>(array[i])
                        << " in " << demangled_name << " at " << exec_basename << std::endl;
                free(demangled_name);
            }
            else
            {
                std::cout << "#" << i
                        << "  0x" << std::setfill('0') << std::setw(16) << std::hex
                        << reinterpret_cast<size_t>(array[i])
                        << " in " << mangled_name << " at " << exec_basename << std::endl;
            }
        }
        else if(match_regex(symbols[i], "(.*)() [\[](.*)[]]", 3,
                NULL,
                &module,
                &address))
        {
            std::cout << "#" << i
                    << "  0x" << std::setfill('0') << std::setw(16) << std::hex
                    << reinterpret_cast<size_t>(array[i])
                    << " in ?? at " << exec_basename << std::endl;
        }
        else
            std::cout << "#" << i << "  " << symbols[i] << std::endl;
    }
    free(symbols);
    exit(0);
}

static void gdb_sighandler(int sig, siginfo_t* info, void* secret)
{
    std::stringstream ss;
    ss << getpid();
    int child_pid = fork();
    if(!child_pid)
    {
        std::string execname = get_execname();
        execlp("gdb", "-n", "-ex", "bt", execname.c_str(), ss.str().c_str(), NULL);
        abort();
    }
    else
        waitpid(child_pid, NULL, 0);
    exit(0);
}

void add_sighandler(int sig, void (*_sa_sigaction)(int, siginfo_t*, void*))
{
    struct sigaction sa;
    sa.sa_sigaction = _sa_sigaction;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART|SA_SIGINFO;
    sigaction(sig, &sa, NULL);
}

int func_a(int a, char b)
{
    char* p = (char*)0xdeadbeef;
    a = a+b;
    *p = 10;
    return 2*a;
}

int func_b()
{
    int a = 5;
    int res = 5+func_a(a, 't');
    return res;
}

int main(int argc, char** argv)
{
    add_sighandler(SIGSEGV, backtrace_sighandler);
    add_sighandler(SIGINT,  backtrace_sighandler);
    add_sighandler(SIGFPE,  backtrace_sighandler);
    add_sighandler(SIGBUS,  backtrace_sighandler);
    add_sighandler(SIGILL,  backtrace_sighandler);
    printf("%d\n", func_b());
    return 0;
}
