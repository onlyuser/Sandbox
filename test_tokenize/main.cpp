// REFERENCE:
// "Nested strtok function problem in C"
// http://stackoverflow.com/questions/4693884/nested-strtok-function-problem-in-c

// CONCEPTS:
// * nested strtok

#include <string.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    char str[] = "q,w,e;a,s,d;z,x,c";
    char* end_str;
    char* token = strtok_r(str, ";", &end_str);
    while(token)
    {
        char* end_token;
        printf("a = %s\n", token);
        char* token2 = strtok_r(token, ",", &end_token);
        while (token2 != NULL)
        {
            printf("b = %s\n", token2);
            token2 = strtok_r(NULL, ",", &end_token);
        }
        token = strtok_r(NULL, ";", &end_str);
    }
    return 0;
}
