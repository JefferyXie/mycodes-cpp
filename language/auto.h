#include "../header.h"

// C++11 auto, how to determine type of auto?
// http://stackoverflow.com/questions/7138588/c11-auto-what-if-it-gets-a-constant-reference
template<class T> class A_auto
{
public:
    static void foo(){ std::cout<< "foo value" << std::endl; }
    static void goo(T v){ std::cout << "goo value" << std::endl; }
    static void koo(T& v){ std::cout << "koo(T&)" << std::endl; }
};
template<class T> class A_auto<T&>
{
public:
    static void foo(){ std::cout<< "foo reference" << std::endl; }
    static void goo(T& v){ std::cout << "goo reference" << std::endl; }
};

float& bar()
{
    static float t=5.5;
    return t;
}


