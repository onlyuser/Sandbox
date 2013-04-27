// REFERENCE:
// http://stackoverflow.com/questions/3151779/how-its-better-to-invoke-gdb-from-program-to-print-its-stacktrace

// DEMONSTRATES:
// * backtrace
// * gdb attach process
// * fork-exec
// * waitpid

// NOTE: for Ubuntu 10.10 or later, must run as root
// http://askubuntu.com/questions/41629/after-upgrade-gdb-wont-attach-to-process

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void print_trace()
{
    char pid_buf[30];
    sprintf(pid_buf, "%d", getpid());
    char name_buf[512];
    name_buf[readlink("/proc/self/exe", name_buf, 511)] = 0;
    int child_pid = fork();
    if(!child_pid)
    {
        dup2(2, 1); // redirect output to stderr
        fprintf(stdout,"stack trace for %s pid=%s\n", name_buf, pid_buf);
        execlp("gdb", "gdb", "--batch", "-n", "-ex", "thread", "-ex", "bt", name_buf, pid_buf, NULL);
        abort(); // if gdb failed to start
    }
    else
        waitpid(child_pid, NULL, 0);
}

void baz()
{
    print_trace();
}

void bar() { baz(); }
void foo() { bar(); }

int main(int argc, char** argv)
{
    foo();
    return 0;
}
