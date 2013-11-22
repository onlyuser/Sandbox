#include "stdio.h"

int g()
{
    return 0;
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

    f(x);
    f(g());
    return 0;
}
