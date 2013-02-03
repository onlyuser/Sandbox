#ifndef MAIN_H_
#define MAIN_H_

#include <boost/variant/static_visitor.hpp>
#include <iostream>

class VisitableIFace;
class ExtendedVisitable;
class ExtendedVisitable2;

struct VisitorIFace : public boost::static_visitor<>
{
    virtual ~VisitorIFace()
    {}
    template<typename T>
    void operator()(T &x) const
    {
        std::cout << "value=" << x << std::endl;
    }
    virtual void dispatch_visit(VisitableIFace* x) const = 0;
};

struct Visitor : public VisitorIFace
{
    virtual void visit(ExtendedVisitable* x) const = 0;
    virtual void visit(ExtendedVisitable2* x) const = 0;
    void dispatch_visit(VisitableIFace* x) const;
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
    virtual void accept(const VisitorIFace* v) = 0;
};

template<class T>
class Visitable : public VisitableIFace
{
public:
    Visitable(T* instance) : m_instance(instance)
    {}
    void accept(const VisitorIFace* v)
    {
        // "Java Tip 98" from http://en.wikipedia.org/wiki/Visitor_pattern
        v->dispatch_visit(m_instance);

        // Alternatively, since the type of m_instance is static, you can do
        // this to the same effect:
        //dynamic_cast<const Visitor*>(v)->visit(m_instance);
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
