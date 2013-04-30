// REFERENCE:
// "How it's better to invoke gdb from program to print its stacktrace?"
// http://stackoverflow.com/questions/3151779/how-its-better-to-invoke-gdb-from-program-to-print-its-stacktrace/4611112#4611112

// CONCEPTS:
// * backtrace
// * line number resolution using addr2line

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <unistd.h>
#include <cxxabi.h>
#include <string.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <regex.h>
#include <stdarg.h>
#include <vector>
#include <sys/wait.h>

//#define __USE_GNU
#include <ucontext.h> // REG_EIP

#define MAX_EXEC_NAME_SIZE 1024
#define MAX_PIPE_BUF_SIZE 128

static std::string get_exec_name()
{
    char buf[MAX_EXEC_NAME_SIZE];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf)-1);
    if(len == -1)
        return "";
    buf[len] = '\0';
    return buf;
}

static std::string system_capture_stdout(std::string cmd)
{
    FILE* file = popen(cmd.c_str(), "r");
    if(!file)
        return "";
    char buf[MAX_PIPE_BUF_SIZE];
    std::string result = "";
    while(!feof(file))
    {
        if(fgets(buf, sizeof(buf), file))
            result += buf;
    }
    pclose(file);
    return result.substr(0, result.length()-1);
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

#ifdef __x86_64__
    #define REG_EIP REG_RIP
#endif

static void backtrace_signal_handler(int sig, siginfo_t* info, void* secret)
{
    std::cout << "stack array for " << get_exec_name() << " pid=" << getpid() << std::endl;
    std::cout << "Error: signal " << sig << ":" << std::endl;
    ucontext_t* uc = (ucontext_t*)secret;
    void* array[16];
    int size = backtrace(array, 16);
    array[1] = reinterpret_cast<void*>(uc->uc_mcontext.gregs[REG_EIP]);
    char** symbols = backtrace_symbols(array, size);
    for(int i = 1; i<size; i++)
    {
        std::stringstream ss;
        ss << "basename `addr2line " << array[i] << " -e " << get_exec_name() << "`";
        std::string exec_basename = system_capture_stdout(ss.str());
        std::string module, mangled_name, offset, address;
        bool regex_status =
                match_regex(symbols[i], "(.*)[\(](.*)[+](.*)[)] [\[](.*)[]]", 5,
                        NULL,
                        &module,
                        &mangled_name,
                        &offset,
                        &address);
        if(regex_status)
        {
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
        else
            std::cout << "#" << i << "  " << symbols[i] << std::endl;
    }
    free(symbols);
    exit(0);
}

static void gdb_signal_handler(int sig, siginfo_t* info, void* secret)
{
    std::stringstream ss;
    ss << getpid();
    int child_pid = fork();
    if(!child_pid)
    {
        std::string exec_name = get_exec_name();
        execlp("gdb", "-n", "-ex", "bt", exec_name.c_str(), ss.str().c_str(), NULL);
        abort();
    }
    else
        waitpid(child_pid, NULL, 0);
    exit(0);
}

void add_signal_handler(int sig, bool launch_gdb)
{
    struct sigaction sa;
    sa.sa_sigaction = launch_gdb ? gdb_signal_handler : backtrace_signal_handler;
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
    add_signal_handler(SIGSEGV, false);
    printf("%d\n", func_b());
    return 0;
}
