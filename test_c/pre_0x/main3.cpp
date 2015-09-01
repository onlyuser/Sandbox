// REFERENCE:
// "Most vexing parse"
// http://en.wikipedia.org/wiki/Most_vexing_parse

#include <typeinfo>
#include <iostream>

int g(int) { return 0; }
struct g2 {};
typedef int (*g3)(int);

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

    // function declaration!! -- because "g2" is a type
    // NOTE: "g2()" is not a default constructor call
    g2 f2(g2());
    std::cout << typeid(f2).name() << std::endl; // g2 (g2 (*)())

    // function declaration!! -- because "g3" is a type
    int f3(g3(x));
    std::cout << typeid(f3).name() << std::endl; // int (int (*)(int))

    return 0;
}
