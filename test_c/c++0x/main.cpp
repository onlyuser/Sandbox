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

    // lvalue is a named reference
    printf("test 1\n");
    f(x);    // lvalue
    f(x+0);  // rvalue
    f(g());  // rvalue
    f(g2()); // lvalue
    f(3);    // rvalue

    // assigning rvalue to a variable automatically converts it to an lvalue (by giving it a name)
    printf("test 2\n");
    auto x2 = x;
    auto x3 = x+0;
    auto x4 = g();
    auto x5 = g2();
    auto x6 = 3;
    f(x2); // lvalue
    f(x3); // lvalue!!
    f(x4); // lvalue!!
    f(x5); // lvalue
    f(x6); // lvalue!!

    // std::move coerces lvalue to rvalue
    printf("test 3\n");
    f(std::move(x)); // rvalue

    // rvalue-ness is not necessarily preserved through a universal reference
    printf("test 4\n");
    f2(x);   // lvalue
    f2(g()); // lvalue!!

    // std::forward preserves rvalue-ness through a universal reference
    printf("test 5\n");
    f3(x);   // lvalue
    f3(g()); // rvalue

    return 0;
}
