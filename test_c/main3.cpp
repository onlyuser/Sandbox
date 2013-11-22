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
    printf("f(int&)\n");
}

void f(int &&x)
{
    printf("f(int&&)\n");
}

int main(int argc, char** argv)
{
    int x;

    f(x);    // lvalue
    f(g());  // rvalue
    f(g2()); // lvalue
    return 0;
}
