// REFERENCE:
// "Parsing a comma-delimited std::string"
// http://stackoverflow.com/questions/1894886/parsing-a-comma-delimited-stdstring

// CONCEPTS:
// * imbue locale with custom facet to cin
// * tokenize with istream_iterator
// * output to standard out

#include <locale>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

struct csv_reader : public std::ctype<char>
{
    csv_reader() : std::ctype<char>(get_table())
    {}
    static std::ctype_base::mask const* get_table()
    {
        static std::vector<std::ctype_base::mask> rc(table_size, std::ctype_base::mask());
        rc[' '] = std::ctype_base::space;
        rc['\n'] = std::ctype_base::space;
        rc[','] = std::ctype_base::space;
        rc[';'] = std::ctype_base::space;
        return &rc[0];
    }
};

int main(int argc, char** argv)
{
    std::cin.imbue(std::locale(std::locale(), new csv_reader()));
    std::copy(std::istream_iterator<int>(std::cin),
            std::istream_iterator<int>(),
            std::ostream_iterator<int>(std::cout, "\n"));
    return 0;
}
