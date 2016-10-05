// REFERENCE:
// "use getline and while loop to split a string"
// http://stackoverflow.com/questions/5757721/use-getline-and-while-loop-to-split-a-string

// CONCEPTS:
// * tokenize with getline
// * output to standard container

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int main(int argc, char** argv)
{
    std::string str("abc:def");
    char split_char = ':';
    std::istringstream split(str);
    std::vector<std::string> tokens;
    for (std::string each; std::getline(split, each, split_char); tokens.push_back(each));
    for(std::vector<std::string>::iterator p = tokens.begin(); p != tokens.end(); p++) {
        std::cout << *p << std::endl;
    }
    return 0;
}
