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

    int y(int(x));
    std::cout << typeid(y).name() << std::endl; // int (int)

    int f(g(x));
    std::cout << typeid(f).name() << std::endl; // int

    int f2(h(x));
    std::cout << typeid(f2).name() << std::endl; // int (int (*)(int))

    qwe f3(qwe());
    std::cout << typeid(f3).name() << std::endl; // qwe (qwe (*)())

    return 0;
}
