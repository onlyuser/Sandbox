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

struct facet : public std::ctype<char>
{
    facet() : std::ctype<char>(get_table())
    {}
    static std::ctype_base::mask const* get_table()
    {
        static std::vector<std::ctype_base::mask> table(table_size, std::ctype_base::mask());
        table[' '] = std::ctype_base::space;
        table['\n'] = std::ctype_base::space;
        table[','] = std::ctype_base::space;
        table[';'] = std::ctype_base::space;
        return &table[0];
    }
};

int main(int argc, char** argv)
{
    std::cin.imbue(std::locale(std::locale(), new facet()));
    std::copy(std::istream_iterator<int>(std::cin),
            std::istream_iterator<int>(),
            std::ostream_iterator<int>(std::cout, "\n"));
    return 0;
}
