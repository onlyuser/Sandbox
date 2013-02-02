#ifndef MAIN_H_
#define MAIN_H_

#include <boost/variant/static_visitor.hpp>
#include <iostream>

class ExtendedVisitable;
class ExtendedVisitable2;

struct Visitor : public boost::static_visitor<>
{
    virtual ~Visitor()
    {}
    template<typename T>
    void operator()(T &x) const
    {
        std::cout << "value=" << x << std::endl;
    }
    virtual void visit(ExtendedVisitable* x) const = 0;
    virtual void visit(ExtendedVisitable2* x) const = 0;
};

struct ExtendedVisitor : public Visitor
{
    void visit(ExtendedVisitable* x) const;
    void visit(ExtendedVisitable2* x) const;
};

struct ExtendedVisitor2 : public Visitor
{
    void visit(ExtendedVisitable* x) const;
    void visit(ExtendedVisitable2* x) const;
};

struct VisitableIFace
{
    virtual ~VisitableIFace()
    {}
    virtual void accept(const Visitor* v) = 0;
};

template<class T>
class Visitable : public VisitableIFace
{
public:
    Visitable(T* instance) : m_instance(instance)
    {}
    // STEP #3: Promote Visitor to dynamic type through "visit" vtable-lookup
    // STEP #4: Simulate promoting Visitee to dynamic type through matching
    //          method-overload of "visit" to template-specialized Visitee
    //          static type (borrowing ideas from CRTP and Java Tip 98)
    void accept(const Visitor* v)
    {
        // "Java Tip 98" from http://en.wikipedia.org/wiki/Visitor_pattern
        v->visit(m_instance);
    }

private:
    T *m_instance;
};

struct ExtendedVisitable : public Visitable<ExtendedVisitable>
{
    ExtendedVisitable() : Visitable<ExtendedVisitable>(this)
    {}
};

struct ExtendedVisitable2 : public Visitable<ExtendedVisitable2>
{
    ExtendedVisitable2() : Visitable<ExtendedVisitable2>(this)
    {}
};

#endif
