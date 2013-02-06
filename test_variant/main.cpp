#include "main.h"
#include <iostream>
#include <boost/variant.hpp>
#include <boost/variant/get.hpp>
#include <string>
#include <typeinfo>

template<>
void VisitorIFace::operator()<VisitableIFace*>(VisitableIFace* &x) const
{
    // Simulate RTTI via double dispatch
    x->accept(this);

    // Alternatively, if RTTI is available, we can do this to the same effect:
    //dispatch_visit(x);
}

void Visitor::dispatch_visit(VisitableIFace* x) const
{
    if(typeid(*x) == typeid(ExtendedVisitable))
        visit(dynamic_cast<ExtendedVisitable*>(x));
    if(typeid(*x) == typeid(ExtendedVisitable2))
        visit(dynamic_cast<ExtendedVisitable2*>(x));
}

void ExtendedVisitor::visit(ExtendedVisitable* x) const
{
    std::cout << "ExtendedVisitor visits ExtendedVisitable" << std::endl;
}

void ExtendedVisitor::visit(ExtendedVisitable2* x) const
{
    std::cout << "ExtendedVisitor visits ExtendedVisitable2" << std::endl;
}

void ExtendedVisitor2::visit(ExtendedVisitable* x) const
{
    std::cout << "ExtendedVisitor2 visits ExtendedVisitable" << std::endl;
}

void ExtendedVisitor2::visit(ExtendedVisitable2* x) const
{
    std::cout << "ExtendedVisitor2 visits ExtendedVisitable2" << std::endl;
}

int main(int argc, char** argv)
{
    boost::variant<int, double, std::string, char, VisitableIFace*> x;
    ExtendedVisitor v;
    ExtendedVisitor2 v2;

    x = 13;
    std::cout << "value=" << boost::get<int>(x) << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(v, x);

    x = 3.14;
    std::cout << "value=" << boost::get<double>(x) << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(v, x);

    x = "hello";
    std::cout << "value=" << boost::get<std::string>(x) << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(v, x);

    x = 'c';
    std::cout << "value=" << boost::get<char>(x) << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(v, x);

    x = new ExtendedVisitable();
    std::cout << "value=" << "0x??" /*boost::get<VisitableIFace*>(x)*/ << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(v, x);

    x = new ExtendedVisitable2();
    std::cout << "value=" << "0x??" /*boost::get<VisitableIFace*>(x)*/ << ", type_id=" << x.which() << std::endl;
    boost::apply_visitor(v2, x);

    return 0;
}
