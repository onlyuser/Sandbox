#include "coroutine/coroutine_cpp.h"
#include <stdio.h>
#include <iostream>

int static_ascending()
{
    static int i;
    scrBegin;
    for(i=0; i<10; i++)
    {
        scrReturn(i);
    }
    scrFinish(-1);
}

int reentrant_ascending(ccrContParam)
{
    ccrBeginContext;
    int i;
    ccrEndContext(foo);

    ccrBegin(foo);
    for(foo->i=0; foo->i<10; foo->i++) {
        ccrReturn(foo->i);
    }
    ccrFinish(-1);
}

void test_static_coroutine()
{
    int i;
    do
    {
        i = static_ascending();
        printf("got number %d\n", i);
    } while(i != -1);
}

void test_reentrant_coroutine()
{
    ccrContext z = 0;
    do
    {
        printf("got number %d\n", reentrant_ascending(&z));
    } while(z);
}

int main(int argc, char** argv)
{
    std::cout << "static coroutine:" << std::endl;
    test_static_coroutine();
    std::cout << std::endl;
    std::cout << "reentrant coroutine:" << std::endl;
    test_reentrant_coroutine();
    return 0;
}
