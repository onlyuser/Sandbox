// http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes

// demonstrates:
// * backtrace
// * signal handling

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <signal.h>

void handler(int sig)
{
    void* array[10];
    size_t size;

    // get void* for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, 2);
    exit(1);
}

void baz()
{
    int* foo = (int*)-1; // make a bad pointer
    printf("%d\n", *foo); // causes segfault
}

void bar() { baz(); }
void foo() { bar(); }

int main(int argc, char** argv)
{
    signal(SIGSEGV, handler); // install our handler
    foo(); // this will call foo, bar, and baz. baz segfaults.
}
