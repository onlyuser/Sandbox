#include <iostream>
#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

int main(int argc, char**argv)
{
    boost::variant<int, std::string, char> v;

    v = 13;
    std::cout << boost::get<int>(v) << ", " << v.which() << std::endl;

    v = "hello";
    std::cout << boost::get<std::string>(v) << ", " << v.which() << std::endl;

    v = 'c';
    std::cout << boost::get<char>(v) << ", " << v.which() << std::endl;
}
