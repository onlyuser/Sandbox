// REFERENCE:
// "How to get file descriptor of buffer in memory?"
// http://stackoverflow.com/questions/1558772/how-to-get-file-descriptor-of-buffer-in-memory

// CONCEPTS:
// * convert in-memory buffer to FILE*
// * convert file descriptor to FILE*
// * pipe std-out from one process to std-in of another process

#include <stdio.h> // fdopen, fprintf, fscanf
#include <unistd.h> // close, fork, pipe
#include <iostream> // std::cout, std::endl
#include <stdlib.h> // atoi, exit
#include <string.h> // strlen

#define BUF_MAX 80

int main()
{
    int p[2];
    pipe(p);
    pid_t child_pid = fork();
    if(!child_pid) { // child process
        close(p[0]);                                    // close read-channel -- we're writing
        FILE* file = fdopen(p[1], "w");                 // open pipe to parent process
        char buf[] = "qwe";                             // prepare data
        fwrite(buf, sizeof(char), strlen(buf)+1, file); // send message to parent process through pipe
        exit(0);                                        // exit to ensure no side-effects
    } else { // parent process
        close(p[1]);                                              // close write-channel -- we're reading
        FILE* file = fdopen(p[0], "r");                           // open pipe to child process
        char buf[BUF_MAX];                                        // prepare storage for data
        fread(buf, sizeof(char), sizeof(buf)/sizeof(char), file); // receive message from child process through pipe
        std::cout << "Result is: " << buf << std::endl;
    }
    return 0;
}
