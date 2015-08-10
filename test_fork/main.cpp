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
    // One practical solution I discovered is to fork the main process and
    // sacrifice the child process by allowing it to enter the corrupted
    // program state, then writing the result to disk so it can later be
    // recovered by the main process, which waits for the child process.
    pid_t child_pid = fork();
    if(!child_pid) {
        // child process
        int result = 0;
        if(!someone_elses_code_with_undesired_side_effect(&result, 2, 3)) {
            exit(1);
        }
        FILE *file = fopen(TEMP_FILENAME, "wb");
        fprintf(file, "%d", result);
        fclose(file);
        exit(0); // no longer need this since we already got what we want
    } else {
        // parent process
        waitpid(child_pid, NULL, 0);
        FILE *file = fopen(TEMP_FILENAME, "rb");
        char buf[MAX_BUF_SIZE] = "";
        fgets(buf, sizeof(buf), file);
        fclose(file);
        remove(TEMP_FILENAME);
        printf("The result is: %s\n", buf);
    }
    someone_elses_code_manifesting_undesired_side_effect();
    return 0;
}
