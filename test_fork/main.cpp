#include <unistd.h> // fork
#include <sys/wait.h> // waitpid
#include <stdlib.h> // exit
#include <stdio.h> // printf

#define TEMP_FILENAME "result.txt"
#define MAX_BUF_SIZE 20

extern bool someone_elses_code_with_undesired_side_effect(int *result, int a, int b);
extern void someone_elses_code_manifesting_undesired_side_effect();

int main(int argc, char** argv)
{
    // PROBLEM:
    // Suppose we need to call "someone_elses_code_with_undesired_side_effect"
    // to do useful work, but by calling
    // "someone_elses_code_with_undesired_side_effect", the program enters a
    // corrupted state, which causes the program to crash upon any subsequent
    // call to "someone_elses_code_manifesting_undesired_side_effect", which we
    // need for later flows. Then how do we work-around this?

    // EXAMPLE:
    // Enable following code to see program crash on call to
    // "someone_elses_code_manifesting_undesired_side_effect".
#if 0
    int result = 0;
    someone_elses_code_with_undesired_side_effect(&result, 2, 3);
    printf("The result is: %d\n", result);
    someone_elses_code_manifesting_undesired_side_effect();
    return 0;
#endif

    // SOLUTION:
    // One practical solution is to fork the main process and sacrifice the
    // child process by allowing it to enter the corrupted program state, then
    // writing the result to disk so it can later be recovered by the main
    // process, which waits for the child process.
    pid_t child_pid = fork();
    if(!child_pid) { // child process
        // Get the result.
        // This corrupts program state.
        // Fortunately, this is the child process.
        int result = 0;
        if(!someone_elses_code_with_undesired_side_effect(&result, 2, 3)) {
            exit(1);
        }

        // write result to disk for main process to recover
        FILE *file = fopen(TEMP_FILENAME, "wb");
        fprintf(file, "%d", result);
        fclose(file);

        exit(0); // no longer needed since we already wrote result to disk
    } else { // parent process
        // wait for child process to exit
        waitpid(child_pid, NULL, 0);

        // read child process result
        FILE *file = fopen(TEMP_FILENAME, "rb");
        char buf[MAX_BUF_SIZE] = "";
        fgets(buf, sizeof(buf), file);
        fclose(file);
        remove(TEMP_FILENAME);

        // print result
        printf("The result is: %s\n", buf);
    }

    // Further down the line, call the innocuous function
    // "someone_elses_code_manifesting_undesired_side_effect" that crashes on
    // corrupt program state. This is safe for the main process, which is what
    // we intended.
    someone_elses_code_manifesting_undesired_side_effect();

    return 0;
}