#include "main.h"
#include <iostream>
#include <boost/variant.hpp>
#include <boost/variant/get.hpp>
#include <string>

// BASIC FLOW: STEP #1:   boost::apply_visitor(Visitor, boost::variant<Visitable*, ...>)
//             STEP #2:   => Visitor::operator()(Visitable*)
//             STEP #3,4: => Visitable::accept(Visitor*)
//                        => Visitor::visit(Visitable*)

// STEP #2: Promote Visitee to dynamic type through "accept" vtable-lookup
template<>
void Visitor::operator()<Visitable*>(Visitable* &x) const
{
    x->accept(this);
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
    boost::variant<int, double, std::string, char, Visitable*> x;
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
    std::cout << "value=" << "0x??" /*boost::get<Visitable*>(x)*/ << ", type_id=" << x.which() << std::endl;
    // STEP #1A: Begin by locking Visitor and Visitee to static type through
    //           boost::apply_visitor magic
    boost::apply_visitor(v, x);

    x = new ExtendedVisitable2();
    std::cout << "value=" << "0x??" /*boost::get<Visitable*>(x)*/ << ", type_id=" << x.which() << std::endl;
    // STEP #1B: Begin by locking Visitor and Visitee to static type through
    //           boost::apply_visitor magic
    boost::apply_visitor(v2, x);

    return 0;
}
