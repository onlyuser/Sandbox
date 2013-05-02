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

class facet : public std::ctype<char>
{
    mask m_table[table_size];
public:
    facet(size_t refs = 0) : std::ctype<char>(&m_table[0], false, refs)
    {
        //std::copy_n(classic_table(), table_size, m_table);
        std::copy(classic_table(), classic_table()+table_size, m_table);
        m_table[','] = (mask)space;
        m_table[';'] = (mask)space;
    }
};

int main(int argc, char** argv)
{
    std::istringstream iss("q,w,e;a,s,d;z,x,c");
    iss.imbue(std::locale(std::locale::classic(), new facet));
    std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(),
            std::ostream_iterator<std::string>(std::cout, "\n"));
    return 0;
}
