// REFERENCE:
// "How to get file descriptor of bufer in memory?"
// http://stackoverflow.com/questions/1558772/how-to-get-file-descriptor-of-bufer-in-memory

// CONCEPTS:
// * convert in-memory bufer to FILE*
// * convert file descriptor to FILE*
// * pipe std-out from one process to std-in of another process
// * sandbox code that corrupts program state

#include <stdio.h> // fdopen, fprintf, fscanf
#include <unistd.h> // close, fork, pipe
#include <iostream> // std::cout, std::endl
#include <stdlib.h> // atoi, exit
#include <signal.h> // raise, SIGSEGV

static bool do_crash = false;

static int do_calculation(int a, int b) {
    do_crash = true;
    return a + b;
}

static void report_result(int x) {
    std::cout << "Result is: " << x << std::endl;
    if(do_crash) {
        raise(SIGSEGV);
    }
}

int main(int argc, char** argv)
{
    if(argc != 3) {
        std::cout << "Error: Expect 2 arguments!" << std::endl;
        return 1;
    }
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int result = 0;
#if 0
    result = do_calculation(a, b);
#else
    int p[2];
    pipe(p);
    pid_t child_pid = fork();
    if(!child_pid) {                    // child process
        close(p[0]);                    // close read-channel -- we're writing
        FILE* file = fdopen(p[1], "w"); // open pipe to parent process
        result = do_calculation(a, b);  // sandbox code that corrupts program state
        fprintf(file, "%d", result);    // send message to parent process through pipe
        exit(0);                        // exit to ensure no side-effects
    } else {                            // parent process
        close(p[1]);                    // close write-channel -- we're reading
        FILE* file = fdopen(p[0], "r"); // open pipe to child process
        fscanf(file, "%d", &result);    // receive message from parent process through pipe
    }
#endif
    report_result(result);
    return 0;
}
