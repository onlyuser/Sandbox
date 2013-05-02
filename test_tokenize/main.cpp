// REFERENCE:
// "Nested strtok function problem in C"
// http://stackoverflow.com/questions/4693884/nested-strtok-function-problem-in-c

// CONCEPTS:
// * nested strtok

#include <string.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    char s[] = "q,w,e;a,s,d;z,x,c";
    char* save_ptr;
    char* token = strtok_r(s, ";", &save_ptr);
    while(token)
    {
        printf("a = %s\n", token);
        char* save_ptr2;
        char* token2 = strtok_r(token, ",", &save_ptr2);
        while(token2)
        {
            printf("b = %s\n", token2);
            token2 = strtok_r(NULL, ",", &save_ptr2);
        }
        token = strtok_r(NULL, ";", &save_ptr);
    }
    return 0;
}
