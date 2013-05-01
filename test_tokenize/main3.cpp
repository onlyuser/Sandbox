// REFERENCE:
// "How can I use non-default delimiters when reading a text file with std::fstream?"
// http://stackoverflow.com/questions/10376199/how-can-i-use-non-default-delimiters-when-reading-a-text-file-with-stdfstream

// CONCEPTS:
// * imbue locale with custom facet to istringstream
// * tokenize with istream_iterator
// * output to standard out

#include <locale>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>

class my_ctype : public std::ctype<char>
{
    mask my_table[table_size];
public:
    my_ctype(size_t refs = 0) : std::ctype<char>(&my_table[0], false, refs)
    {
        std::copy(classic_table(), classic_table()+table_size, my_table);
        my_table[','] = (mask)space;
        my_table[';'] = (mask)space;
    }
};

int main(int argc, char** argv)
{
    std::istringstream input("q,w,e;a,s,d;z,x,c");
    std::locale x(std::locale::classic(), new my_ctype);
    input.imbue(x);
    std::copy(std::istream_iterator<std::string>(input),
            std::istream_iterator<std::string>(),
            std::ostream_iterator<std::string>(std::cout, "\n"));
    return 0;
}
