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

char* exe = 0;

static void initialiseExecutableName()
{
    char link[1024];
    exe = new char[1024];
    snprintf(link, sizeof(link), "/proc/%d/exe", getpid());
    if(readlink(link, exe, sizeof(link)) == -1)
    {
        fprintf(stderr, "ERRORRRRR\n");
        exit(1);
    }
}

static const char* getExecutableName()
{
    if(!exe)
        initialiseExecutableName();
    return exe;
}

static std::string _exec(std::string cmd)
{
    FILE* pipe = popen(cmd.c_str(), "r");
    if(!pipe)
        return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe))
    {
        if(fgets(buffer, 128, pipe))
            result += buffer;
    }
    pclose(pipe);
    return result;
}

static bool match_regex(std::string s, std::string pattern, int n, ...)
{
    std::vector<std::string*> results;
    va_list ap;
    va_start(ap, n);
    for(int i = 0; i<n; i++)
        results.push_back(va_arg(ap, std::string*));
    va_end(ap);
    regex_t regex;
    if(regcomp(&regex, pattern.c_str(), REG_ICASE|REG_EXTENDED))
        return false;
    regmatch_t* regmatch = new regmatch_t[n];
    if(!regmatch)
        return false;
    int status = regexec(&regex, s.c_str(), n, regmatch, 0);
    regfree(&regex);
    if(!status)
    {
        for(int i = 0; i<n; i++)
        {
            if(results[i])
                *results[i] = s.substr(regmatch[i].rm_so, regmatch[i].rm_eo-regmatch[i].rm_so);
        }
    }
    delete[] regmatch;
    return true;
}

// get REG_EIP from ucontext.h
//#define __USE_GNU
#include <ucontext.h>

#ifdef __x86_64__
    #define REG_EIP REG_RIP
#endif

void bt_sighandler(int sig, siginfo_t* info, void* secret)
{
    std::cout << "stack trace for " << getExecutableName() << " pid=" << getpid() << std::endl;

    ucontext_t* uc = (ucontext_t*)secret;

    // do something useful with siginfo_t
    if(sig == SIGSEGV)
    {
        printf("Got signal %d, faulty address is %p, from %ld\n",
                sig, info->si_addr, uc->uc_mcontext.gregs[REG_EIP]);
    }
    else
        printf("Got signal %d\n", sig);

    void* trace[16];
    int trace_size = backtrace(trace, 16);

    // overwrite sigaction with caller's address
    trace[1] = reinterpret_cast<void*>(uc->uc_mcontext.gregs[REG_EIP]);

    char** messages = backtrace_symbols(trace, trace_size);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    // skip first stack frame (points here)
    for(int i=1; i<trace_size; ++i)
    {
        std::stringstream ss;
        // last parameter is the name of this app
        ss << "basename `addr2line " << trace[i] << " -e " << getExecutableName() << "`";
        std::string filename = _exec(ss.str());
        filename = filename.substr(0, filename.length()-1);

        std::string module, proc, offset, address;
        bool regex_status =
                match_regex(messages[i], "(.*)[\(](.*)[+](.*)[)] [\[](.*)[]]", 5,
                        NULL,
                        &module,
                        &proc,
                        &offset,
                        &address);

        if(regex_status)
        {
            int status;
            char* ret = abi::__cxa_demangle(proc.c_str(), funcname, &funcnamesize, &status);
            if(status == 0)
            {
                funcname = ret; // use possibly realloc()-ed string
                std::cout << "#" << i
                        << "  0x" << std::setfill('0') << std::setw(16) << std::hex
                        << reinterpret_cast<size_t>(trace[i])
                        << " in " << funcname << " at " << filename << std::endl;
            }
            else
            {
                // demangling failed. Output function name as a C function with
                // no arguments.
                std::cout << "#" << i
                        << "  0x" << std::setfill('0') << std::setw(16) << std::hex
                        << reinterpret_cast<size_t>(trace[i])
                        << " in " << proc << " at " << filename << std::endl;
            }
        }
        else
        {
            // couldn't parse the line? print the whole line.
            std::cout << "#" << i << "  " << messages[i] << std::endl;
        }
    }
    exit(0);
}

int func_a(int a, char b)
{
    char* p = (char*)0xdeadbeef;
    a = a+b;
    *p = 10; // CRASH here!!
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
    // install our signal handler
    struct sigaction sa;

    sa.sa_sigaction = bt_sighandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART|SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    // ... add any other signal here

    // do something
    printf("%d\n", func_b());
    return 0;
}
