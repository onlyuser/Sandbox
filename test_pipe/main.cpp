// REFERENCE:
// "How to get file descriptor of bufer in memory?"
// http://stackoverflow.com/questions/1558772/how-to-get-file-descriptor-of-bufer-in-memory

// CONCEPTS:
// * convert in-memory bufer to FILE*
// * convert file descriptor to FILE*
// * pipe std-out from one process to std-in of another process

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

int main()
{
    int p[2];
    pipe(p);
    pid_t child_pid = fork();
    if(!child_pid) { // parent process
        close(p[0]); // close read-channel -- indicates we're writing
        FILE* file = fdopen(p[1], "w");
        char buf[] = "qwe\nasd\n";
        fwrite(buf, sizeof(char), strlen(buf)+1, file);
    } else {         // child process
        close(p[1]); // close write-channel -- indicates we're reading
        FILE* file = fdopen(p[0], "r");
        size_t n = 100;
        char buf[n];
        fread(buf, sizeof(char), n, file);
        std::cout << "result: \"" << buf << "\"" << std::endl;
    }
    return 0;
}
