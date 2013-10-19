// REFERENCE:
// "How do you capture a group with regex?"
// http://stackoverflow.com/questions/2577193/how-do-you-capture-a-group-with-regex

// CONCEPTS:
// * posix regex
// * capturing regex groups

#include <regex.h>
#include <stdarg.h>
#include <vector>
#include <sstream>
#include <iostream>

std::string replace(std::string &s, std::string find_string, std::string replace_string)
{
    if(s.empty() || find_string.empty())
        return s;
    std::string _s(s);
    for(size_t p = 0; (p = _s.find(find_string, p)) != std::string::npos; p += replace_string.length())
         _s.replace(p, find_string.length(), replace_string);
    return _s;
}

bool regexp(std::string &s, std::string pattern, int nmatch, std::vector<std::string*> &cap_groups)
{
    if(!nmatch)
        return false;
    regex_t preg;
    if(regcomp(&preg, pattern.c_str(), REG_ICASE|REG_EXTENDED))
        return false;
    regmatch_t* pmatch = new regmatch_t[nmatch];
    if(!pmatch)
        return false;
    if(regexec(&preg, s.c_str(), nmatch, pmatch, 0))
    {
        delete[] pmatch;
        regfree(&preg);
        return false;
    }
    regfree(&preg);
    for(int i = 0; i<nmatch; i++)
    {
        std::string* ptr = cap_groups[i];
        if(ptr)
            *ptr = s.substr(pmatch[i].rm_so, pmatch[i].rm_eo-pmatch[i].rm_so);
    }
    delete[] pmatch;
    return true;
}

bool regexp(std::string &s, std::string pattern, int nmatch, ...)
{
    if(!nmatch)
        return false;
    std::vector<std::string*> args(nmatch);
    va_list ap;
    va_start(ap, nmatch);
    for(int i = 0; i<nmatch; i++)
        args[i] = va_arg(ap, std::string*);
    va_end(ap);
    return regexp(s, pattern, nmatch, args);
}

bool regsub(std::string &s, std::string pattern, int nmatch, std::string replace_string)
{
    const int MAX_CAP_GROUPS = 10;
    std::vector<std::string> cap_groups(MAX_CAP_GROUPS);
    std::vector<std::string*> cap_groups_ref(MAX_CAP_GROUPS);
    int i = 0;
    for(auto p = cap_groups.begin(); p != cap_groups.end(); p++)
    {
        cap_groups_ref[i] = &(*p);
        i++;
    }
    bool result = false;
    size_t p = 0;
    while(regexp(s, pattern, nmatch, cap_groups_ref))
    {
        result = true;
        if((p = s.find(cap_groups[0], p)) == std::string::npos)
            break;
        std::string temp_replace_string = replace_string;
        for(int j = 1; j<nmatch; j++)
        {
            std::stringstream ss;
            ss << "\\" << j;
            temp_replace_string = replace(temp_replace_string, ss.str(), cap_groups[j]);
        }
        s.replace(p, cap_groups[0].length(), temp_replace_string);
        p += temp_replace_string.length();
    }
    return result;
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "wrong number of arguments!" << std::endl;
        return 1;
    }
    std::string s = argv[1];
    regsub(s, "(.*) [\(](.*)[+](.*)[)] [\[](.*)[]]", 5, "\\4 (\\3+\\2) [\\1]");
    std::cout << s << std::endl;
    return 0;
}
