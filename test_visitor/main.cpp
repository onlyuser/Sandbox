#include "main.h"
#include <iostream>

void VisitorDFS::visit(InnerNode* inner_node) const
{
    for(int i=0; i<static_cast<int>(inner_node->size()); i++) // default traversal
    {
        if(inner_node->child(i))
            inner_node->child(i)->accept(this);
    }
    std::cout << "name=\"" << inner_node->name() << '\"' << std::endl;
}

void VisitorDFS::visit(TermNode* term_node) const
{
    std::cout << "name=\"" << term_node->name() << '\"' << std::endl;
}

void ReverseVisitorDFS::visit(InnerNode* inner_node) const
{
    for(int i=static_cast<int>(inner_node->size())-1; i >=0; i--) // reverse traversal
    {
        if(inner_node->child(i))
            inner_node->child(i)->accept(this);
    }
    std::cout << "[reverse] name=\"" << inner_node->name() << '\"' << std::endl;
}

void ReverseVisitorDFS::visit(TermNode* term_node) const
{
    std::cout << "[reverse] ";
    VisitorDFS::visit(term_node);
}

int main(int argc, char** argv)
{
    InnerNode inner_node(4, "parent");
    inner_node.child(0)->name() = "child_0";
    inner_node.child(1)->name() = "child_1";
    inner_node.child(2)->name() = "child_2";
    inner_node.child(3)->name() = "child_3";

    VisitorDFS v;
    inner_node.accept(&v);

    ReverseVisitorDFS v2;
    inner_node.accept(&v2);

    return 0;
}
