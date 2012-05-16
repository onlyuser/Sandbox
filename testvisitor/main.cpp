#include "main.h"
#include <iostream>
#include <sstream>

static std::string ptr_to_string(const void* x)
{
    std::stringstream ss;
    ss << '_' << x;
    std::string s = ss.str();
    return s;
}

void ThingVisitor::visit(Thing* thing)
{
	std::cout << m_indent_string << '{' << std::endl;
	enter_scope();
    for(int i=0; i<4; i++)
    {
    	thing->m_thing[i].accept(this);
    }
    leave_scope();
	std::cout << m_indent_string << '}' << std::endl;
	std::cout << m_indent_string << "visit Thing: " << ptr_to_string(thing) << std::endl;
}

void ThingVisitor::visit(ThingElem* thing_elem)
{
	std::cout << m_indent_string << "visit ThingElem: " << ptr_to_string(thing_elem) << std::endl;
}

void ThingVisitor::dispatch_visit(TypeIdent* unknown)
{
	switch(unknown->m_type)
	{
	case TypeIdent::TYPE_THING:
		visit(dynamic_cast<Thing*>(unknown));
		break;
	case TypeIdent::TYPE_THING_ELEM:
		visit(dynamic_cast<ThingElem*>(unknown));
		break;
	}
}

void ThingVisitor2::visit(Thing* thing)
{
	std::cout << m_indent_string << "ThingVisitor2::visit(Thing*) {" << std::endl;
	enter_scope();
	ThingVisitor::visit(thing);
	leave_scope();
	std::cout << m_indent_string << '}' << std::endl;
}

void ThingVisitor2::visit(ThingElem* thing_elem)
{
	std::cout << m_indent_string << "ThingVisitor2::visit(ThingElem*) {" << std::endl;
	enter_scope();
	ThingVisitor::visit(thing_elem);
	leave_scope();
	std::cout << m_indent_string << '}' << std::endl;
}

int main()
{
    Thing thing;
    ThingVisitor2 v;
    thing.accept(&v);
}
