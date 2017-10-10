#include <iostream> 
#include <memory>

// [X] marks incompatible lhs-rhs pairs
// +-----------------+-----------------+-----------------+
// |                 |    non-const    |      const      |
// |     lhs\rhs     +-----+-----+-----+-----+-----+-----+
// |                 | T&  |  T  | T&& | T&  |  T  | T&& |
// +-----------+-----+-----+-----+-----+-----+-----+-----+
// |           | T&  |     |  X  |  X  |  X  |  X  |  X  |
// |           +-----+-----+-----+-----+-----+-----+-----+
// | non-const | T   |     |     |     |  X  |  X  |  X  |
// |           +-----+-----+-----+-----+-----+-----+-----+
// |           | T&& |  X  |     |     |  X  |  X  |  X  |
// +-----------+-----+-----+-----+-----+-----+-----+-----+
// |           | T&  |     |     |     |     |     |     |
// |           +-----+-----+-----+-----+-----+-----+-----+
// |   const   | T   |     |     |     |     |     |     |
// |           +-----+-----+-----+-----+-----+-----+-----+
// |           | T&& |  X  |     |     |  X  |     |     |
// +-----------+-----+-----+-----+-----+-----+-----+-----+

class Foo
{
public:
    Foo()
    {
        //std::cout << "\tFoo::Foo()" << std::endl;
    }
    Foo(const Foo& other)
    {
        //std::cout << "\tFoo::Foo(const Foo&)" << std::endl;
    }
    ~Foo()
    {
        //std::cout << "\tFoo::~Foo()" << std::endl;
    }
    Foo& operator=(const Foo& other)
    {
        //std::cout << "\tFoo& Foo::operator=(const Foo&)" << std::endl;
        return *this;
    }
};

//=============================================================================
// auto_ptr

//std::auto_ptr<Foo> f()
//{
//    return std::auto_ptr<Foo>(new Foo);
//}

//=============================================================================
// rhs

// non-const
Foo& rhs_lvalue_ref()
{
    static Foo a;
    return a;
}

Foo rhs_rvalue()
{
    return Foo();
}

#define rhs_rvalue_ref() std::move(rhs_rvalue())

// const
const Foo& rhs_const_lvalue_ref()
{
    static Foo a;
    return a;
}

const Foo rhs_const_rvalue()
{
    return Foo();
}

#define rhs_const_rvalue_ref() std::move(rhs_const_rvalue())

//=============================================================================
// lhs

// non-const
void lhs_lvalue_ref(Foo& other)
{
    std::cout << "\tlhs_lvalue_ref(Foo& other)" << std::endl;
}

void lhs_rvalue(Foo other)
{
    std::cout << "\tlhs_rvalue(Foo other)" << std::endl;
}

void lhs_rvalue_ref(Foo&& other)
{
    std::cout << "\tlhs_rvalue_ref(Foo&& other)" << std::endl;
}

// const
void lhs_const_lvalue_ref(const Foo& other)
{
    std::cout << "\tlhs_const_lvalue_ref(const Foo& other)" << std::endl;
}

void lhs_const_rvalue(const Foo other)
{
    std::cout << "\tlhs_const_rvalue(const Foo other)" << std::endl;
}

void lhs_const_rvalue_ref(const Foo&& other)
{
    std::cout << "\tlhs_const_rvalue_ref(const Foo&& other)" << std::endl;
}

//=============================================================================
// universal reference

// non-const
void lhs_detect(Foo& other)
{
    std::cout << "\tlhs_detect(Foo& other)" << std::endl;
}

// NOTE: ambiguous with Foo&
//void lhs_detect(Foo other)
//{
//    std::cout << "\tlhs_detect(Foo other)" << std::endl;
//}

void lhs_detect(Foo&& other)
{
    std::cout << "\tlhs_detect(Foo&& other)" << std::endl;
}

template<class T>
void lhs_unversal_ref(T&& other)
{
    std::cout << typeid(other).name() << std::endl;
    lhs_detect(std::forward<T>(other));
}

// const
void lhs_const_detect(const Foo& other)
{
    std::cout << "\tlhs_const_detect(const Foo& other)" << std::endl;
}

void lhs_const_detect(const Foo other)
{
    std::cout << "\tlhs_const_detect(const Foo other)" << std::endl;
}

void lhs_const_detect(const Foo&& other)
{
    std::cout << "\tlhs_const_detect(const Foo&& other)" << std::endl;
}

template<class T>
void lhs_const_unversal_ref(const T&& other)
{
    std::cout << typeid(other).name() << std::endl;
    lhs_const_detect(std::forward<T>(other));
}

//=============================================================================

int main(int argc, char** argv)
{
    //===============
    // lhs_lvalue_ref
    //===============

    // non-const
    std::cout << "lhs_lvalue_ref(rhs_lvalue_ref())" << std::endl;
    lhs_lvalue_ref(rhs_lvalue_ref());

    std::cout << "lhs_lvalue_ref(rhs_rvalue())" << std::endl;
    std::cout << "\terror: invalid initialization of non-const reference of type ‘Foo&’ from an rvalue of type ‘Foo’" << std::endl;
    //lhs_lvalue_ref(rhs_rvalue());

    std::cout << "lhs_lvalue_ref(rhs_rvalue_ref())" << std::endl;
    std::cout << "\terror: invalid initialization of non-const reference of type ‘Foo&’ from an rvalue of type ‘std::remove_reference<Foo>::type {aka Foo}’" << std::endl;
    //lhs_lvalue_ref(rhs_rvalue_ref());

    // const
    std::cout << "lhs_const_lvalue_ref(rhs_lvalue_ref())" << std::endl;
    lhs_const_lvalue_ref(rhs_lvalue_ref());

    std::cout << "lhs_const_lvalue_ref(rhs_rvalue())" << std::endl;
    lhs_const_lvalue_ref(rhs_rvalue());

    std::cout << "lhs_const_lvalue_ref(rhs_rvalue_ref())" << std::endl;
    lhs_const_lvalue_ref(rhs_rvalue_ref());

    //===========
    // lhs_rvalue
    //===========

    // non-const
    std::cout << "lhs_rvalue(rhs_lvalue_ref())" << std::endl;
    lhs_rvalue(rhs_lvalue_ref());

    std::cout << "lhs_rvalue(rhs_rvalue())" << std::endl;
    lhs_rvalue(rhs_rvalue());

    std::cout << "lhs_rvalue(rhs_rvalue_ref())" << std::endl;
    lhs_rvalue(rhs_rvalue_ref());

    // const
    std::cout << "lhs_const_rvalue(rhs_lvalue_ref())" << std::endl;
    lhs_const_rvalue(rhs_lvalue_ref());

    std::cout << "lhs_const_rvalue(rhs_rvalue())" << std::endl;
    lhs_const_rvalue(rhs_rvalue());

    std::cout << "lhs_const_rvalue(rhs_rvalue_ref())" << std::endl;
    lhs_const_rvalue(rhs_rvalue_ref());

    //===============
    // lhs_rvalue_ref
    //===============

    // non-const
    std::cout << "lhs_rvalue_ref(rhs_lvalue_ref())" << std::endl;
    std::cout << "\terror: cannot bind ‘Foo’ lvalue to ‘Foo&&’" << std::endl;
    //lhs_rvalue_ref(rhs_lvalue_ref());

    std::cout << "lhs_rvalue_ref(rhs_rvalue())" << std::endl;
    lhs_rvalue_ref(rhs_rvalue());

    std::cout << "lhs_rvalue_ref(rhs_rvalue_ref())" << std::endl;
    lhs_rvalue_ref(rhs_rvalue_ref());

    // const
    std::cout << "lhs_const_rvalue_ref(rhs_lvalue_ref())" << std::endl;
    std::cout << "\terror: cannot bind ‘Foo’ lvalue to ‘const Foo&&’" << std::endl;
    //lhs_const_rvalue_ref(rhs_lvalue_ref());

    std::cout << "lhs_const_rvalue_ref(rhs_rvalue())" << std::endl;
    lhs_const_rvalue_ref(rhs_rvalue());

    std::cout << "lhs_const_rvalue_ref(rhs_rvalue_ref())" << std::endl;
    lhs_const_rvalue_ref(rhs_rvalue_ref());

    //=================
    // lhs_unversal_ref
    //=================

    // non-const
    std::cout << "lhs_unversal_ref(rhs_lvalue_ref())" << std::endl;
    lhs_unversal_ref(rhs_lvalue_ref());

    std::cout << "lhs_unversal_ref(rhs_rvalue())" << std::endl;
    lhs_unversal_ref(rhs_rvalue());

    std::cout << "lhs_unversal_ref(rhs_rvalue_ref())" << std::endl;
    lhs_unversal_ref(rhs_rvalue_ref());

    // const
    std::cout << "lhs_const_unversal_ref(rhs_lvalue_ref())" << std::endl;
    std::cout << "\terror: cannot bind ‘Foo’ lvalue to ‘const Foo&&’" << std::endl;
    //lhs_const_unversal_ref(rhs_lvalue_ref());

    std::cout << "lhs_const_unversal_ref(rhs_rvalue())" << std::endl;
    std::cout << "\terror: no matching function for call to ‘forward(const Foo&)" << std::endl;
    //lhs_const_unversal_ref(rhs_rvalue());

    std::cout << "lhs_const_unversal_ref(rhs_rvalue_ref())" << std::endl;
    std::cout << "\terror: no matching function for call to ‘forward(const Foo&)’" << std::endl;
    //lhs_const_unversal_ref(rhs_rvalue_ref());

    return 0;
}
