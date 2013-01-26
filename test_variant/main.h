#ifndef MAIN_H_
#define MAIN_H_

#include <boost/variant/static_visitor.hpp>
#include <iostream>

class MyVisitor : public boost::static_visitor<>
{
public:
	template<typename T>
    void operator()(T &x) const
	{
	    std::cout << "value=" << x << std::endl;
	}
};

#endif
