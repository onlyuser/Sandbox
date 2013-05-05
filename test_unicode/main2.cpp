// REFERENCE:
// "Using the Boost code facet for reading UTF8"
// http://www.edobashira.com/2010/03/using-boost-code-facet-for-reading-utf8.html

// CONCEPTS:
// * difference between utf-imbued ifstream and non-utf-imbued ifstream read length

#include "utf8_codecvt_facet.hpp" // utf8_codecvt_facet
#include <iostream> // std::cout
#include <fstream> // std::wifstream
#include <locale> // std::locale

int main(int argc, char** argv)
{
    std::wifstream wifs("utf8.txt");
    wifs.imbue(std::locale(std::locale(), new utf8_codecvt_facet));
    std::wstring wstr;
    wifs >> wstr;
    wifs.close();
    std::cout << "wstr.length()=" << wstr.length() << std::endl;
    std::ifstream ifs("utf8.txt");
    std::string str;
    ifs >> str;
    std::cout << "str.length()=" << str.length() << std::endl;
    ifs.close();
    return 0;
}
