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

bool match_regex(std::string s, std::string pattern, int nmatch, ...)
{
    bool result = true;
    regex_t preg;
    if(regcomp(&preg, pattern.c_str(), REG_ICASE|REG_EXTENDED)) {
        return false;
    }
    regmatch_t* pmatch = new regmatch_t[nmatch];
    if(pmatch) {
        int status = regexec(&preg, s.c_str(), nmatch, pmatch, 0);
        regfree(&preg);
        if(!status) {
            va_list ap;
            va_start(ap, nmatch);
            for(int i = 0; i<nmatch; i++)
            {
                std::string* ptr = va_arg(ap, std::string*);
                if(ptr)
                    *ptr = s.substr(pmatch[i].rm_so, pmatch[i].rm_eo-pmatch[i].rm_so);
            }
            va_end(ap);
        } else {
            result = false;
        }
        delete[] pmatch;
    } else {
        result = false;
    }
    return result;
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        std::cout << "wrong number of arguments!" << std::endl;
        return 1;
    }
    std::string s[5];
    match_regex(argv[1], "(.*)[\(](.*)[+](.*)[)] [\[](.*)[]]", 5, &s[0], &s[1], &s[2], &s[3], &s[4]);
    for(int i = 0; i<5; i++) {
        std::cout << "match #" << i << ": " << s[i] << std::endl;
    }
    return 0;
}
