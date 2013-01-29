#include "main.h"
#include <iostream>
#include <boost/variant.hpp>
#include <boost/variant/get.hpp>
#include <string>

template<>
void ExtendedVisitor::operator()<Node*>(Node* &x) const
{
	x->accept(this);
}

int main(int argc, char**argv)
{
    boost::variant<int, double, std::string, char, Node*> x;

    x = 13;
    std::cout << "value=" << boost::get<int>(x) << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(ExtendedVisitor(), x);

    x = 3.14;
    std::cout << "value=" << boost::get<double>(x) << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(ExtendedVisitor(), x);

    x = "hello";
    std::cout << "value=" << boost::get<std::string>(x) << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(ExtendedVisitor(), x);

    x = 'c';
    std::cout << "value=" << boost::get<char>(x) << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(ExtendedVisitor(), x);

    x = new Node();
    std::cout << "value=" << boost::get<Node*>(x) << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(ExtendedVisitor(), x);

    x = new ExtendedNode();
    std::cout << "value=" << boost::get<Node*>(x) << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(ExtendedVisitor(), x);
}
