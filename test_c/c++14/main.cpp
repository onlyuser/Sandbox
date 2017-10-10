// REFERENCE:
// "C++ Rvalue References Explained"
// http://thbecker.net/articles/rvalue_references/section_01.html

// "Universal References in C++11"
// https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers

// "Who ordered rvalue references? Part 2"
// https://bartoszmilewski.com/2008/10/26/who-ordered-rvalue-references-part-2/

// "Copy elision"
// http://en.cppreference.com/w/cpp/language/copy_elision

// "Efficiency of C++11 push_back() with std::move versus emplace_back() for already constructed objects"
// https://stackoverflow.com/questions/26860749/efficiency-of-c11-push-back-with-stdmove-versus-emplace-back-for-already

// "C++, rvalue references in function parameters"
// https://stackoverflow.com/questions/30339922/c-rvalue-references-in-function-parameters

// CONCEPTS:
// * rvalue reference
// * copy elision (return value optimization)
// * emplace

#include <iostream>
#include <vector>
#include <string.h>
#include <utility>
#include <memory>

#define BIG_NUMBER 1000

class A
{
public:
    A()
        : m_buf(NULL)
    {
        std::cout << "A::A()" << std::endl;
        m_buf = new char[BIG_NUMBER];
        m_buf[0] = '\0';
    }

    A(const char* buf)
        : m_buf(NULL)
    {
        std::cout << "A::A(\"" << buf << "\")" << std::endl;
        m_buf = new char[BIG_NUMBER];
        strncpy(m_buf, buf, sizeof(char) * BIG_NUMBER);
    }

    A(const A& other)
        : m_buf(NULL)
    {
        std::cout << "A::A(const A&)" << std::endl;
        m_buf = new char[BIG_NUMBER];
        strncpy(m_buf, other.m_buf, sizeof(char) * BIG_NUMBER);
    }

    A(A&& other)
        : m_buf(NULL)
    {
        std::cout << "A::A(A&&)" << std::endl;
        std::swap(m_buf, other.m_buf);
    }

    A(A&& other, int)
        : m_buf(NULL)
    {
        std::cout << "A::A(A&&, int)" << std::endl;
        std::swap(m_buf, other.m_buf);
    }

    ~A()
    {
        if(m_buf) {
            std::cout << "A::~A(\"" << m_buf << "\")" << std::endl;
            delete []m_buf;
            return;
        }
        std::cout << "A::~A()" << std::endl;
    }

    const A& operator=(A& other)
    {
        std::cout << "A::operator=(const A& other)" << std::endl;
        m_buf = new char[BIG_NUMBER];
        strncpy(m_buf, other.m_buf, sizeof(char) * BIG_NUMBER);
        return *this;
    }

    const A& operator=(A&& other)
    {
        std::cout << "A::operator=(const A&& other)" << std::endl;
        std::swap(m_buf, other.m_buf);
        return *this;
    }

private:
    char* m_buf;
};

static A the_a("the a");

//void f0(A& a)       { std::cout << "void f0(A&)" << std::endl; }

void f(A& a)        { std::cout << "void f(A&)" << std::endl; }
void f(const A& a)  { std::cout << "void f(const A&)" << std::endl; }

void f2(A& a)       { std::cout << "void f2(A&)" << std::endl; }
void f2(const A& a) { std::cout << "void f2(const A&)" << std::endl; }
void f2(A&& a)      { std::cout << "void f2(A&&)" << std::endl; }

A g()               { return the_a; }

template<class T>
void f2_forward(T&& other) { f2(std::forward<T>(other)); }

int main(int argc, char** argv)
{
    // NOTE: won't compile
    //std::cout << "\ntest0" << std::endl;
    //{
    //    f0(g()); // rvalue won't bind to "A&"
    //}

    std::cout << "\ntest1" << std::endl;
    {
        f(the_a); // lvalue reference prefers "A&"
        f(g());   // rvalue reference prefers "const A&" over "A&"
    }

    std::cout << "\ntest2" << std::endl;
    {
        f2(the_a); // lvalue reference prefers "A&"
        f2(g());   // rvalue reference prefers "A&&" over "const A&"
    }

    std::cout << "\ntest3" << std::endl;
    {
        f2_forward(the_a); // preserves rvalueness despite rvalue collapse rule
        f2_forward(g());   // preserves rvalueness despite rvalue collapse rule
    }

    std::cout << "\ntest4" << std::endl;
    {
        std::shared_ptr<::A> shared_a = std::make_shared<::A>("outer");

        std::cout << "ENTER SCOPE" << std::endl;
        {
            auto unique_a = std::make_unique<::A>("inner");
            shared_a = std::move(unique_a); // exchanges ownership with outer
        }
        std::cout << "LEAVE SCOPE" << std::endl;
    }

    std::cout << "\ntest5" << std::endl;
    {
        ::A a1("outer");

        std::cout << "ENTER SCOPE" << std::endl;
        {
            ::A a2("inner");
            std::swap(a1, a2); // swap works as you'd expect; exchanges ownership with outer
        }
        std::cout << "LEAVE SCOPE" << std::endl;
    }

    std::cout << "\ntest6" << std::endl;
    {
        A(A(A("hello world"))); // anonymous construction of object of type A; RVO is default behavior unless use gcc switch -fno-elide-constructors
    }

    std::cout << "\ntest7" << std::endl;
    {
        A(A(A(), 0), 0); // anonymous construction of object of type A; RVO not possible because "A(A(A()))" pattern is broken
    }

    std::cout << "\ntest8" << std::endl;
    {
        {
            A(A(A())); // center A is local variable declaration of function pointer type
            std::cout << typeid(A).name() << std::endl; // A (A (*)())
        }

        {
            typedef A(A(A())); // center A is function pointer type
            std::cout << typeid(A).name() << std::endl; // A (A (*)())
        }
    }

    std::cout << "\ntest9" << std::endl;
    {
        std::vector<::A> v;
        v.emplace_back("hello world"); // emplace is unrelated to rvalue reference
    }

    std::cout << "\ntest10" << std::endl;
    {
        std::shared_ptr<A> a = std::make_shared<A>("hello world"); // std::make_shared is unrelated to rvalue reference
    }

    return 0;
}
