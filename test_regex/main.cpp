// REFERENCE:
// "How do you capture a group with regex?"
// http://stackoverflow.com/questions/2577193/how-do-you-capture-a-group-with-regex

// CONCEPTS:
// * posix regex
// * capturing regex groups

#include <stdio.h>
#include <regex.h>

#define SENDER_REGEX "MAIL FROM:<(.*)>"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("wrong number of arguments!\n");
        return 1;
    }
    regex_t regex;
    if(regcomp(&regex, SENDER_REGEX, REG_ICASE|REG_EXTENDED))
        printf("regcomp error\n");
    regmatch_t pmatch[2];
    int status = regexec(&regex, argv[1], 2, pmatch, 0);
    regfree(&regex);
    if(!status)
        printf("matched from %d (%c) to %d (%c)\n",
                pmatch[1].rm_so, argv[1][pmatch[1].rm_so],
                pmatch[1].rm_eo, argv[1][pmatch[1].rm_eo]);
    return 0;
}
