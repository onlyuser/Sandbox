#include <utility>
#include "stdio.h"

int g()
{
    return 0;
}

int &g2()
{
    static int x;
    return x;
}

//void f(int x)
//{
//    printf("f(int)\n");
//}

void f(int &x)
{
    printf("lvalue\n");
}

void f(int &&x)
{
    printf("rvalue\n");
}

template<class T>
void f2(T&& arg)
{
    f(arg);
}

template<class T>
void f3(T&& arg)
{
    f(std::forward<T>(arg));
}

int main(int argc, char** argv)
{
    int x;

    // lvalue is everything that is named
    // rvalue is a temporary waiting to be unwinded from the stack, or a literal
    printf("test 1\n");
    f(x);   // lvalue
    f(g()); // rvalue
    f(3);   // rvalue

    // Assigning rvalue to a variable automatically converts it to an lvalue (by giving it a name)
    printf("test 2\n");
    auto x2 = x;
    auto y2 = g();
    auto z2 = 3;
    f(x2); // lvalue
    f(y2); // lvalue!
    f(z2); // lvalue!

    // Return values can also be lvalue, but we can use std::move to coerce it back to rvalue
    printf("test 3\n");
    f(g2());            // lvalue
    f(std::move(g2())); // rvalue

    // rvalue-ness is not preserved through a template
    printf("test 4\n");
    f2(x);   // lvalue
    f2(g()); // lvalue!

    // rvalue-ness can only be preserved through a template using std::forward
    printf("test 5\n");
    f3(x);   // lvalue
    f3(g()); // rvalue

    return 0;
}
