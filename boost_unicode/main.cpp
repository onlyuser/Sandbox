#include <iostream>
#include <fstream>
#include <locale>
#include <vector>
#include <iterator>

#define BOOST_UTF8_BEGIN_NAMESPACE
#define BOOST_UTF8_END_NAMESPACE
#define BOOST_UTF8_DECL

#include <boost/detail/utf8_codecvt_facet.hpp>
#include "libs/detail/utf8_codecvt_facet.cpp"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "requires 1 argument" << std::endl;
        return -1;
    }

    std::wstring ucs4_data = L"日本語 + abc + 123";
    std::string filename = argv[1];

    // My encoding type
    typedef wchar_t ucs4_t;

    std::locale old_locale;
    std::locale utf8_locale(old_locale, new utf8_codecvt_facet/*<ucs4_t>*/);
    std::locale loc1;

    // Set a New global locale
    std::locale::global(utf8_locale);

    // Send the UCS-4 data out, converting to UTF-8
    {
        std::wofstream ofs(filename.c_str());
        ofs.imbue(utf8_locale);
        std::copy(ucs4_data.begin(), ucs4_data.end(), std::ostream_iterator<ucs4_t, ucs4_t>(ofs));
    }

    // Read the UTF-8 data back in, converting to UCS-4 on the way in
    std::vector<ucs4_t> from_file;
    {
        std::wifstream ifs(filename.c_str());
        ifs.imbue(utf8_locale);
        ucs4_t item = 0;
        while (ifs >> std::noskipws >> item)
            from_file.push_back(item);
    }

    std::cout << "first traverse" << std::endl;
    int i = 0;
    for(std::vector<ucs4_t>::iterator p = from_file.begin(); p != from_file.end(); p++, i++)
    {
        wchar_t str1[2];
        str1[0] = *p;
        str1[1] = L'\0';
        char str2[2];
        std::use_facet<std::ctype<wchar_t> >(loc1).narrow(str1, str1 + wcslen(str1), 'X', &str2[0]);
        std::cout << "s[" << std::dec << i << "]: 0x"
                << std::hex << static_cast<unsigned long>(*p) << " == "
                        << '\'' << /*static_cast<char>(*p)*/ str2[0] << '\'' << std::endl;
    }
    std::cout << std::endl;

    std::cout << "second traverse" << std::endl;
    std::ios::sync_with_stdio(false);
    std::wcout.imbue(utf8_locale);
    int j = 0;
    for(std::vector<ucs4_t>::iterator p = from_file.begin(); p != from_file.end(); p++, j++)
    {
        std::wcout << L"s[" << std::dec << j << L"]: 0x"
                << std::hex << static_cast<unsigned long>(*p) << L" == "
                        << L'{' << static_cast<wchar_t>(*p) << L'}' << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
