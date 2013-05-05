// REFERENCE:
// "utf8_codecvt_facet"
// http://www.boost.org/doc/libs/1_53_0/libs/serialization/doc/codecvt.html

// CONCEPTS:
// * convert utf32 to utf8
// * convert utf8 to utf32

#include "utf8_codecvt_facet.hpp" // utf8_codecvt_facet
#include <iostream> // std::cout
#include <fstream> // std::wifstream
#include <vector> // std::vector
#include <algorithm> // std::copy
#include <iterator> // std::iterator
#include <locale> // std::locale
#include <stdlib.h> // system

int main(int argc, char** argv)
{
    // my encoding type
    typedef wchar_t ucs4_t;

    std::locale utf8_locale(std::locale(), new utf8_codecvt_facet);

    // set a new global locale
    std::locale::global(utf8_locale);

    // prepare input
    std::vector<ucs4_t> ucs4_data;
    ucs4_data.push_back(L'中');
    ucs4_data.push_back(L'文');

    // send the ucs-4 data out, converting to utf-8
    {
        std::wofstream ofs("build/data.ucd");
        ofs.imbue(utf8_locale);
        std::copy(ucs4_data.begin(), ucs4_data.end(),
                std::ostream_iterator<ucs4_t, ucs4_t>(ofs));
    }

    // read the utf-8 data back in, converting to ucs-4 on the way in
    std::vector<ucs4_t> from_file;
    {
        std::wifstream ifs("build/data.ucd");
        ifs.imbue(utf8_locale);
        ucs4_t item = 0;
        while(ifs >> item)
            from_file.push_back(item);
    }

    // cleanup
    system("rm build/data.ucd");

    // verify output
    assert(from_file[0] == L'中');
    assert(from_file[1] == L'文');

    return 0;
}
