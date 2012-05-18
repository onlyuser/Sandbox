#ifndef MAIN_H_
#define MAIN_H_

#include <string>

struct TypeIdent
{
    typedef enum
    {
        TYPE_THING,
        TYPE_THING_ELEM,
    } type;

    type m_type;

    TypeIdent(type _type) : m_type(_type)
    {}
    virtual ~TypeIdent()
    {}
};

struct Visitor
{
    virtual ~Visitor()
    {}
    virtual void dispatch_visit(TypeIdent* unknown) = 0;
};

template<class T>
struct Visitable
{
    T &m_ref_inst;

    Visitable(T* elem) : m_ref_inst(*elem)
    {}
    virtual ~Visitable()
    {}
    virtual void accept(Visitor* v)
    {
        v->dispatch_visit(&m_ref_inst);
    }
};

struct ThingElem : public TypeIdent, public Visitable<ThingElem>
{
    ThingElem() : TypeIdent(TypeIdent::TYPE_THING_ELEM), Visitable<ThingElem>(this)
    {}
};

struct Thing : public TypeIdent, public Visitable<Thing>
{
    ThingElem m_thing[4];

    Thing() : TypeIdent(TypeIdent::TYPE_THING), Visitable<Thing>(this)
    {}
};

struct ThingVisitor : public Visitor
{
    size_t m_depth;
    std::string m_indent_string;

    ThingVisitor() : m_depth(0)
    {}
    virtual void visit(Thing* thing);
    virtual void visit(ThingElem* thing_elem);
    void dispatch_visit(TypeIdent* unknown);
    void enter_scope()
    {
        m_depth++;
        m_indent_string = std::string(m_depth*4, ' ');
    }
    void leave_scope()
    {
        m_depth--;
        m_indent_string = std::string(m_depth*4, ' ');
    }
};

struct ThingVisitor2 : public ThingVisitor
{
    void visit(Thing* thing);
    void visit(ThingElem* thing_elem);
};

#endif
