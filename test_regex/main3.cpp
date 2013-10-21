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

bool regexp(std::string &s, std::string pattern, std::vector<std::string*> &cap_groups, size_t *start_pos)
{
    int nmatch = cap_groups.size();
    if(!nmatch)
        return false;
    size_t _start_pos(start_pos ? *start_pos : 0);
    std::string rest = s.substr(_start_pos, s.length()-_start_pos);
    regex_t preg;
    if(regcomp(&preg, pattern.c_str(), REG_ICASE|REG_EXTENDED))
        return false;
    regmatch_t* pmatch = new regmatch_t[nmatch];
    if(!pmatch)
        return false;
    if(regexec(&preg, rest.c_str(), nmatch, pmatch, 0))
    {
        delete[] pmatch;
        regfree(&preg);
        return false;
    }
    regfree(&preg);
    for(int i = 0; i<nmatch && cap_groups[i]; i++)
        *(cap_groups[i]) = rest.substr(pmatch[i].rm_so, pmatch[i].rm_eo-pmatch[i].rm_so);
    if(start_pos)
        *start_pos = _start_pos+pmatch[0].rm_so;
    delete[] pmatch;
    return true;
}

bool regexp(std::string &s, std::string pattern, std::vector<std::string*> &cap_groups)
{
    return regexp(s, pattern, cap_groups, NULL);
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
    return regexp(s, pattern, args);
}

bool regsub(std::string &s, std::string pattern, int nmatch, std::string replace_string)
{
    const int MAX_CAP_GROUPS = 10;
    int cap_group_count = std::min(nmatch+1, MAX_CAP_GROUPS);
    std::vector<std::string>  cap_groups(cap_group_count);
    std::vector<std::string*> cap_groups_ref(cap_group_count);
    for(int i = 0; i<cap_group_count; i++)
        cap_groups_ref[i] = &(cap_groups[i]);
    bool result = false;
    size_t pos = 0;
    while(regexp(s, pattern, cap_groups_ref, &pos))
    {
        result = true;
        std::string _replace_string(replace_string);
        for(int j = 0; j<cap_group_count; j++)
        {
            char buf[] = "\\0";
            buf[1] = '0'+j;
            _replace_string = replace(_replace_string, buf, cap_groups[j]);
        }
        s.replace(pos, cap_groups[0].length(), _replace_string);
        pos += _replace_string.length();
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
    regsub(s, "([a-z]+)[\(]([a-z]+)[+]([0-9]+)[)] [\[]([a-z]+)[]]", 4, "\\4(\\3+\\2) [\\1]");
    std::cout << s << std::endl;
    return 0;
}
