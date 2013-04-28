// REFERENCE:
// "How do you capture a group with regex?"
// http://stackoverflow.com/questions/2577193/how-do-you-capture-a-group-with-regex

// CONCEPTS:
// * posix regex
// * capturing regex groups

#include <regex.h>
#include <stdarg.h>
#include <vector>
#include <iostream>

static bool match_regex(std::string s, std::string pattern, int n, ...)
{
    std::vector<std::string*> results;
    va_list ap;
    va_start(ap, n);
    for(int i = 0; i<n; i++)
        results.push_back(va_arg(ap, std::string*));
    va_end(ap);
    regex_t regex;
    if(regcomp(&regex, pattern.c_str(), REG_ICASE|REG_EXTENDED))
        return false;
    regmatch_t* regmatch = new regmatch_t[n];
    if(!regmatch)
        return false;
    int status = regexec(&regex, s.c_str(), n, regmatch, 0);
    regfree(&regex);
    if(!status)
    {
        for(int i = 0; i<n; i++)
        {
            if(results[i])
                *results[i] = s.substr(regmatch[i].rm_so, regmatch[i].rm_eo-regmatch[i].rm_so);
        }
    }
    delete[] regmatch;
    return true;
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "wrong number of arguments!" << std::endl;
        return 1;
    }
    std::string s[5];
    match_regex(argv[1], "(.*)[\(](.*)[+](.*)[)] [\[](.*)[]]", 5, &s[0], &s[1], &s[2], &s[3], &s[4]);
    for(int i = 0; i<5; i++)
        std::cout << "match #" << i << ": " << s[i] << std::endl;
    return 0;
}
