#ifndef MAIN_H_
#define MAIN_H_

#include <boost/variant/static_visitor.hpp>
#include <iostream>

typedef boost::static_visitor<> Visitor;

struct Visitable
{
	virtual ~Visitable()
	{}
	virtual void accept(const Visitor* v) = 0;
};

struct Node : public Visitable
{
	void accept(const Visitor* v);
};

struct ExtendedNode : public Node
{
	void accept(const Visitor* v);
};

class ExtendedVisitor : public Visitor
{
public:
	template<typename T>
    void operator()(T &x) const
	{
	    std::cout << "value=" << x << std::endl;
	}
};

#endif
