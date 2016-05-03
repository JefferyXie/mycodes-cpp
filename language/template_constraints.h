#ifndef TEMPLATE_CONSTRAINTS_H
#define TEMPLATE_CONSTRAINTS_H

#include "../main/header.h"

// 1) by utilizing pointer to function, detect the constraint at compile time
//
// https://isocpp.org/wiki/faq/templates
// http://www.stroustrup.com/bs_faq2.html#constraints

template<class T, class U> struct Derived_from {
    static void constraints(T* p) { U* pb = p; }
    Derived_from() { void(*p)(T*) = constraints; }
};
template<class T1, class T2> struct Can_copy {
    static void constraints(T1 a, T2 b) { T2 c = a; b = a; }
    Can_copy() { void(*p)(T1,T2) = constraints; }
};
template<class T1, class T2 = T1> struct Can_compare {
    static void constraints(T1 a, T2 b) { a == b; a > b; a < b; a != b; }
    Can_compare() { void(*p)(T1,T2) = constraints; }
};
template<class T1, class T2, class T3 = T1> struct Can_multiply {
    static void constraints(T1 a, T2 b, T3 c) { c = a*b; }
    Can_multiply() { void(*p)(T1,T2,T3) = constraints; }
};

struct B_ {};
struct D : B_ {};
struct DD : D {};
struct X {};

// class template
template<class T> class Container : Derived_from<T, B_> {
    // ...
};
// function template
template<class T> void RunDerived(const T& o) {
    // accept object of only B or derived from B_
    Derived_from<T, B_>();

    const B_* pB = &o;
    cout << sizeof(*pB) << endl;
}

void Sample1() {
    Derived_from<D, B_>();
    Derived_from<DD, B_>();
//    Derived_from<X, B_>(); // compile error
//    Derived_from<int, B_>(); // compile error
//    Derived_from<X, int>(); // compile error

    Container<B_> bc;
    Container<D> dc;
    Container<DD> ddc;
//    Container<X> xc; // compile error
//    Container<int> ic; // compile error

    RunDerived(B_());
    RunDerived(DD());
//    RunDerived(X()); // compile error
}

// 
// 2) c++11 type traits can also handle template constraints
// http://stackoverflow.com/questions/874298/c-templates-that-accept-only-certain-types
// http://pfultz2.com/blog/2014/08/17/type-requirements/

// class template
template<class T>
class ContainerB {
    static_assert(std::is_base_of<B_, T>::value, "T must inherit from B_");
    // ...
};

// function template with constainted type as int
template<class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
void increment(T& x) { x++; }

// the return type (bool) is only valid if T is integral type
template<class T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
is_odd(T i) { return bool(i % 2); }

// the second template argument is only valid if T is integral type
template<class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
bool 
is_even(T i) { return !bool(i % 2); }

void Sample2() {
    ContainerB<B_> b;
    ContainerB<DD> dd;
    //ContainerB<X> x; // compile error
    //ContainerB<int> i; // compile error

    int a = 10;
    float f = 5.55f;
    increment(a);
    //increment(f); // compile error
    is_odd(a);
    //is_odd(f); // compile error
    is_even(a);
    //is_even(f); // compile error
}

#endif


