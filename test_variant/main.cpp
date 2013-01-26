#include "main.h"
#include <iostream>
#include <boost/variant.hpp>
#include <boost/variant/get.hpp>
#include <string>

int main(int argc, char**argv)
{
    boost::variant<int, double, std::string, char> v;

    v = 13;
    std::cout << "value=" << boost::get<int>(v) << ", type_id=" << v.which() << std::endl;
    boost::apply_visitor(MyVisitor(), v);

    v = 3.14f;
    std::cout << "value=" << boost::get<double>(v) << ", type_id=" << v.which() << std::endl;
    boost::apply_visitor(MyVisitor(), v);

    v = "hello";
    std::cout << "value=" << boost::get<std::string>(v) << ", type_id=" << v.which() << std::endl;
    boost::apply_visitor(MyVisitor(), v);

    v = 'c';
    std::cout << "value=" << boost::get<char>(v) << ", type_id=" << v.which() << std::endl;
    boost::apply_visitor(MyVisitor(), v);
}
