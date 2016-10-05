// REFERENCE:
// "Splitting a string in C++"
// http://stackoverflow.com/questions/236129/splitting-a-string-in-c

// CONCEPTS:
// * tokenize with istream_iterator
// * output to standard container

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

int main(int argc, char** argv)
{
    std::istringstream iss("The quick brown fox jumps over the lazy dog");
//    std::copy(std::istream_iterator<std::string>(iss),
//            std::istream_iterator<std::string>(),
//            std::ostream_iterator<std::string>(std::cout, "\n"));
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(),
            std::back_inserter<std::vector<std::string> >(tokens));
    for(std::vector<std::string>::iterator p = tokens.begin(); p != tokens.end(); p++) {
        std::cout << *p << std::endl;
    }
    return 0;
}
