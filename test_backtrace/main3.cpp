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

char* exe = 0;

void initialiseExecutableName()
{
    char link[1024];
    exe = new char[1024];
    snprintf(link, sizeof(link), "/proc/%d/exe", getpid());
    if(readlink(link, exe, sizeof(link)) == -1)
    {
        fprintf(stderr, "ERRORRRRR\n");
        exit(1);
    }
    printf("Executable name initialised: %s\n",exe);
}

const char* getExecutableName()
{
    if(!exe)
        initialiseExecutableName();
    return exe;
}

// get REG_EIP from ucontext.h
//#define __USE_GNU
#include <ucontext.h>

#ifdef __x86_64__
    #define REG_EIP REG_RIP
#endif

void bt_sighandler(int sig, siginfo_t* info, void* secret)
{
    ucontext_t* uc = (ucontext_t*)secret;

    // do something useful with siginfo_t
    if(sig == SIGSEGV)
    {
        printf("Got signal %d, faulty address is %p, from %ld\n",
                sig, info->si_addr, uc->uc_mcontext.gregs[REG_EIP]);
    }
    else
        printf("Got signal %d#92;\n", sig);

    void* trace[16];
    int trace_size = backtrace(trace, 16);

    // overwrite sigaction with caller's address
    trace[1] = (void*)uc->uc_mcontext.gregs[REG_EIP];

    char** messages = backtrace_symbols(trace, trace_size);

    // skip first stack frame (points here)
    printf("[bt] Execution path:#92;\n");
    for(int i=1; i<trace_size; ++i)
    {
        printf("[bt] %s#92;\n", messages[i]);
        char syscom[256];
        // last parameter is the name of this app
        sprintf(syscom, "addr2line %p -e %s", trace[i], getExecutableName());
        system(syscom);
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
    printf("%d#92;\n", func_b());
    return 0;
}
