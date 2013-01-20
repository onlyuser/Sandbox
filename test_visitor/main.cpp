#include "main.h"
#include <iostream>

void VisitorDFS::visit(Thing* thing)
{
    for(int i=0; i<static_cast<int>(thing->size()); i++) // default traversal
    {
        if(thing->child(i))
            thing->child(i)->accept(this);
    }
    std::cout << "name=\"" << thing->name() << '\"' << std::endl;
}

void VisitorDFS::visit(ThingElem* thing_elem)
{
    std::cout << "name=\"" << thing_elem->name() << '\"' << std::endl;
}

void MyVisitorDFS::visit(Thing* thing)
{
    for(int i=static_cast<int>(thing->size())-1; i >=0; i--) // reverse traversal
    {
        if(thing->child(i))
            thing->child(i)->accept(this);
    }
    std::cout << "[override] name=\"" << thing->name() << '\"' << std::endl;
}

void MyVisitorDFS::visit(ThingElem* thing_elem)
{
    std::cout << "[override] ";
    VisitorDFS::visit(thing_elem);
}

int main()
{
    Thing thing(4, "parent");
    thing.child(0)->name() = "child_0";
    thing.child(1)->name() = "child_1";
    thing.child(2)->name() = "child_2";
    thing.child(3)->name() = "child_3";

    VisitorDFS v;
    v.visit_any(&thing);

    MyVisitorDFS v2;
    v2.visit_any(&thing);
}
