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

//#define DEBUG

static std::string argv0()
{
    char buf[1024];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf)-1);
    if(len == -1)
        return "";
    buf[len] = '\0';
    return buf;
}

static std::string system_capture_stdout(std::string cmd)
{
    FILE* pipe = popen(cmd.c_str(), "r");
    if(!pipe)
        return "ERROR";
    char pipe_buf[128];
    std::string result = "";
    while(!feof(pipe))
    {
        if(fgets(pipe_buf, sizeof(pipe_buf), pipe))
            result += pipe_buf;
    }
    pclose(pipe);
    return result;
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void launch_gdb_session()
{
    char pid_buf[30];
    sprintf(pid_buf, "%d", getpid());
    char name_buf[512];
    name_buf[readlink("/proc/self/exe", name_buf, 511)] = 0;
    int child_pid = fork();
    if(!child_pid)
    {
        execlp("gdb", "-n", "-ex", "bt", name_buf, pid_buf, NULL);
        abort(); // if gdb failed to start
    }
    else
        waitpid(child_pid, NULL, 0);
}

// get REG_EIP from ucontext.h
//#define __USE_GNU
#include <ucontext.h>

#ifdef __x86_64__
    #define REG_EIP REG_RIP
#endif

void bt_sighandler(int sig, siginfo_t* info, void* secret)
{
    std::cout << "stack array for " << argv0() << " pid=" << getpid() << std::endl;
    std::cout << "Error: signal " << sig << ":" << std::endl;
    ucontext_t* uc = (ucontext_t*)secret;
    void* array[16];
    int size = backtrace(array, 16);
    array[1] = reinterpret_cast<void*>(uc->uc_mcontext.gregs[REG_EIP]);
    char** symbols = backtrace_symbols(array, size);
    for(int i = 1; i<size; i++)
    {
        std::stringstream ss;
        ss << "basename `addr2line " << array[i] << " -e " << argv0() << "`";
        std::string filename = system_capture_stdout(ss.str());
        filename = filename.substr(0, filename.length()-1);
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
                        << " in " << demangled_name << " at " << filename << std::endl;
                free(demangled_name);
            }
            else
            {
                std::cout << "#" << i
                        << "  0x" << std::setfill('0') << std::setw(16) << std::hex
                        << reinterpret_cast<size_t>(array[i])
                        << " in " << mangled_name << " at " << filename << std::endl;
            }
        }
        else
            std::cout << "#" << i << "  " << symbols[i] << std::endl;
    }
    free(symbols);
#ifdef DEBUG
    launch_gdb_session();
#endif
    exit(0);
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
    struct sigaction sa;
    sa.sa_sigaction = bt_sighandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART|SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    printf("%d\n", func_b());
    return 0;
}
