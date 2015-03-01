// REFERENCE:
// "Most vexing parse"
// http://en.wikipedia.org/wiki/Most_vexing_parse

#include <typeinfo>
#include <iostream>

int g(int)
{
    return 0;
}

typedef int (*h)(int);

struct qwe
{
};

int main(int argc, char** argv)
{
    int x = 0;

    // function declaration!! -- because "int(x)" is interpreted as "int x"
    // NOTE: "int(x)" is not a typecast
    int y(int(x));
    std::cout << typeid(y).name() << std::endl; // int (int)

    // variable initialization -- because "g" is not a type
    int f(g(x));
    std::cout << typeid(f).name() << std::endl; // int

    // function declaration!! -- because "h" is a type
    int f2(h(x));
    std::cout << typeid(f2).name() << std::endl; // int (int (*)(int))

    // function declaration!! -- because "qwe" is a type
    // NOTE: "qwe()" is not a default constructor call
    qwe f3(qwe());
    std::cout << typeid(f3).name() << std::endl; // qwe (qwe (*)())

    return 0;
}
