#ifndef MAIN_H_
#define MAIN_H_

#include <string>

class TypeIdent
{
public:
    typedef enum
    {
        TYPE_THING,
        TYPE_THING_ELEM,
    } type_t;

    TypeIdent(type_t _type, std::string name) : m_type(_type), m_name(name)
    {}
    virtual ~TypeIdent()
    {}
    TypeIdent::type_t type() const
    {
        return m_type;
    }
    std::string &name()
    {
        return m_name;
    }

private:
    type_t m_type;
    std::string m_name;
};

struct Visitor
{
    virtual ~Visitor()
    {}
    virtual void dispatch_visit(TypeIdent* unknown) = 0;
};

template<class T>
class Visitable
{
public:
    Visitable(T* elem) : m_ref_inst(*elem)
    {}
    virtual ~Visitable()
    {}
    virtual void accept(Visitor* v)
    {
        v->dispatch_visit(&m_ref_inst);
    }

private:
    T &m_ref_inst;
};

struct ThingElem : public TypeIdent, public Visitable<ThingElem>
{
    ThingElem(std::string name = "")
        : TypeIdent(TypeIdent::TYPE_THING_ELEM, name), Visitable<ThingElem>(this)
    {}
};

struct Thing : public TypeIdent, public Visitable<Thing>
{
public:
    Thing(size_t n, std::string name = "")
        : TypeIdent(TypeIdent::TYPE_THING, name), Visitable<Thing>(this), m_size(n)
    {
        m_thing = new ThingElem*[n];
        for(int i=0; i<static_cast<int>(n); i++)
            m_thing[i] = new ThingElem();
    }
    ~Thing()
    {
        if(m_thing)
        {
            for(int i=0; i<static_cast<int>(m_size); i++)
            {
                if(m_thing[i])
                    delete m_thing[i];
            }
            delete[] m_thing;
        }
    }
    ThingElem* child(int index)
    {
        return m_thing[index];
    }
    size_t size() const
    {
        return m_size;
    }

private:
    ThingElem** m_thing;
    size_t m_size;
};

struct ThingVisitor : public Visitor
{
    ThingVisitor()
    {}
    virtual void visit(Thing* thing);
    virtual void visit(ThingElem* thing_elem);
    void dispatch_visit(TypeIdent* unknown)
    {
        switch(unknown->type())
        {
        case TypeIdent::TYPE_THING:
            visit(dynamic_cast<Thing*>(unknown));
            break;
        case TypeIdent::TYPE_THING_ELEM:
            visit(dynamic_cast<ThingElem*>(unknown));
            break;
        }
    }
};

struct ThingVisitorOverride : public ThingVisitor
{
    void visit(Thing* thing);
    void visit(ThingElem* thing_elem);
};

#endif
