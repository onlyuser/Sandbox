#ifndef MAIN_H_
#define MAIN_H_

#include <string>

class TypeIdentIface
{
public:
    typedef enum
    {
        TYPE_THING,
        TYPE_THING_ELEM,
    } type_t;

    TypeIdentIface(type_t _type, std::string name) : m_type(_type), m_name(name)
    {}
    virtual ~TypeIdentIface()
    {}
    TypeIdentIface::type_t type() const
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

struct VisitorIFace
{
    virtual ~VisitorIFace()
    {}
    virtual void visit_any(TypeIdentIface* unknown) = 0;
};

template<class T>
class Visitable
{
public:
    Visitable(T* instance) : m_instance(*instance)
    {}
    virtual ~Visitable()
    {}
    virtual void accept(VisitorIFace* v)
    {
        v->visit_any(&m_instance);
    }

private:
    T &m_instance;
};

struct ThingElem : public TypeIdentIface, public Visitable<ThingElem>
{
    ThingElem(std::string name = "")
        : TypeIdentIface(TypeIdentIface::TYPE_THING_ELEM, name), Visitable<ThingElem>(this)
    {}
};

struct Thing : public TypeIdentIface, public Visitable<Thing>
{
public:
    Thing(size_t n, std::string name = "")
        : TypeIdentIface(TypeIdentIface::TYPE_THING, name), Visitable<Thing>(this), m_size(n)
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

struct VisitorDFS : public VisitorIFace
{
    VisitorDFS()
    {}
    virtual void visit(Thing* thing);
    virtual void visit(ThingElem* thing_elem);
    void visit_any(TypeIdentIface* unknown)
    {
        switch(unknown->type())
        {
        case TypeIdentIface::TYPE_THING:
            visit(dynamic_cast<Thing*>(unknown));
            break;
        case TypeIdentIface::TYPE_THING_ELEM:
            visit(dynamic_cast<ThingElem*>(unknown));
            break;
        }
    }
};

struct ReverseVisitorDFS : public VisitorDFS
{
    void visit(Thing* thing);
    void visit(ThingElem* thing_elem);
};

#endif
