#include "main.h"
#include <iostream>

void ThingVisitor::visit(Thing* thing)
{
    for(int i=0; i<static_cast<int>(thing->size()); i++) // default traversal
    {
        if(thing->child(i))
            thing->child(i)->accept(this);
    }
    std::cout << "name=\"" << thing->name() << '\"' << std::endl;
}

void ThingVisitor::visit(ThingElem* thing_elem)
{
    std::cout << "name=\"" << thing_elem->name() << '\"' << std::endl;
}

void ThingVisitorOverride::visit(Thing* thing)
{
    for(int i=static_cast<int>(thing->size())-1; i >=0; i--) // reverse traversal
    {
        if(thing->child(i))
            thing->child(i)->accept(this);
    }
    std::cout << "[override] name=\"" << thing->name() << '\"' << std::endl;
}

void ThingVisitorOverride::visit(ThingElem* thing_elem)
{
    std::cout << "[override] ";
    ThingVisitor::visit(thing_elem);
}

int main()
{
    // set up
    Thing thing(4, "parent");
    thing.child(0)->name() = "child_0";
    thing.child(1)->name() = "child_1";
    thing.child(2)->name() = "child_2";
    thing.child(3)->name() = "child_3";

    // visit
    ThingVisitor v;
    thing.accept(&v);

    // visit override
    ThingVisitorOverride v2;
    thing.accept(&v2);
}
