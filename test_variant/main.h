#ifndef MAIN_H_
#define MAIN_H_

#include <boost/variant/static_visitor.hpp>
#include <iostream>

// DESIGN GOALS:
// 1) Visitor visits Visitee
// 2) Visitor is aware of its own dynamic type and the Visitee dynamic type
// 3) Visitee has no knowledge of Visitor

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

struct Visitable
{
	virtual ~Visitable()
	{}
	virtual void accept(const Visitor* v) = 0;
};

template<class T>
struct VisitableImplement : public Visitable
{
	T *m_instance;

	VisitableImplement(T* instance) : m_instance(instance)
	{}
	void accept(const Visitor* v)
	{
		// "Java Tip 98" from http://en.wikipedia.org/wiki/Visitor_pattern
	    v->visit(m_instance);
	}
};

struct ExtendedVisitable : public VisitableImplement<ExtendedVisitable>
{
	ExtendedVisitable() : VisitableImplement<ExtendedVisitable>(this)
	{}
};

struct ExtendedVisitable2 : public VisitableImplement<ExtendedVisitable2>
{
	ExtendedVisitable2() : VisitableImplement<ExtendedVisitable2>(this)
	{}
};

#endif
