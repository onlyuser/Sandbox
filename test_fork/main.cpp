// REFERENCE:
// "How to get file descriptor of bufer in memory?"
// http://stackoverflow.com/questions/1558772/how-to-get-file-descriptor-of-bufer-in-memory

// CONCEPTS:
// * sandbox code that corrupts program state
// * convert in-memory bufer to FILE*
// * convert file descriptor to FILE*
// * pipe std-out from one process to std-in of another process

#include <stdio.h> // fdopen, fprintf, fscanf
#include <unistd.h> // close, fork, pipe
#include <iostream> // std::cout, std::endl
#include <stdlib.h> // atoi, exit
#include <signal.h> // raise, SIGSEGV

static bool corrupted_program_state = false;

static int do_calculation(int a, int b) {
    corrupted_program_state = true;
    return a + b;
}

static void report_result(int x) {
    std::cout << "Result is: " << x << std::endl;
    if(corrupted_program_state) {
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
    result = do_calculation(a, b); // execute code that corrupts program state
#else
    int p[2];
    pipe(p);
    pid_t child_pid = fork(); // create sandbox to isolate risky operations
    if(!child_pid) { // child process
        close(p[0]);                    // close read-channel -- we're writing
        FILE* file = fdopen(p[1], "w"); // open pipe to parent process
        result = do_calculation(a, b);  // execute code that corrupts program state
        fprintf(file, "%d", result);    // send result to parent process through pipe
        exit(0);                        // exit to ensure no side-effects
    } else { // parent process
        close(p[1]);                    // close write-channel -- we're reading
        FILE* file = fdopen(p[0], "r"); // open pipe to child process
        fscanf(file, "%d", &result);    // receive result from child process through pipe
    }
#endif
    report_result(result); // manifest crash if in corrupted program state
    return 0;
}
