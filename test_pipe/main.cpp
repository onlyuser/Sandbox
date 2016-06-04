// REFERENCE:
// "How to get file descriptor of buffer in memory?"
// http://stackoverflow.com/questions/1558772/how-to-get-file-descriptor-of-buffer-in-memory

// CONCEPTS:
// * convert in-memory buffer to FILE*
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
        char write_buff[] = "qwe\nasd\n";
        fwrite(write_buff, sizeof(char), strlen(write_buff)+1, file);
    } else {         // child process
        close(p[1]); // close write-channel -- indicates we're reading
        FILE* file = fdopen(p[0], "r");
        char read_buff[100];
        fread(read_buff, sizeof(read_buff), 1, file);
        std::cout << "result: \"" << read_buff << "\"" << std::endl;
    }
    return 0;
}
