#include "../main/header.h"
#include "./classtest.h"

// C++11 auto, how to determine type of auto?
// http://stackoverflow.com/questions/7138588/c11-auto-what-if-it-gets-a-constant-reference
// http://aristeia.com/Papers/appearing%20and%20disappearing%20consts.pdf
// http://www.cprogramming.com/c++11/c++11-auto-decltype-return-value-after-function.html
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

Plain plain_obj() {
    return Plain();
}
const Plain plain_const() {
    return Plain();
}
const Plain& plain_const_ref() {
    static Plain ca;
    return ca;
}
// auto won't carry the reference (&) but will carry const and pointer
void Run_auto() {
    cout << "Plain a;" << endl;
    cout << "Plain& b = a;" << endl;
    Plain a;
    Plain& b = a;
    cout << "auto a1 = a" << endl;
    auto a1 = a; // a1 is Plain
    cout << "auto a2 = b" << endl;
    auto a2 = b; // a2 is Plain
    cout << "auto& a3 = b" << endl;
    auto& a3 = b;// a3 is Plain&
    cout << "const auto& a4 = b" << endl;
    const auto& a4 = b; // a4 is const Plain&

    cout << "auto a5 = plain_obj()" << endl;
    auto a5 = plain_obj(); // a5 is Plain
    cout << "auto a6 = plain_const()" << endl;
    auto a6 = plain_const(); // a6 is const Plain
    cout << "auto a7 = plain_const_ref()" << endl;
    auto a7 = plain_const_ref(); // a7 is const Plain
    cout << "auto& a8 = plain_const_ref()" << endl;
    auto& a8 = plain_const_ref();// a8 is const Plain&
}

