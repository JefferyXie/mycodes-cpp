#pragma once

#include "../core/header.h"

//
// constraints
//
// v1: traits as return type
template <typename T>
typename std::enable_if_t<std::is_integral_v<T>, int> count_one_bits(T arg)
{
    //...
    return 0;
}

// v2: traits as dummy template argument
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>, void>>
int count_one_bits(T arg)
{
    //...
    return 0;
}

// v3: 'requires' with traits
template <typename T>
    requires std::is_integral_v<T>
int count_one_bits(T arg)
{
    //...
    return 0;
}

// v4: 'requires' a concept and decoramte the return type
template <typename T>
    requires std::integral<T>
int count_one_bits(T arg)
{
    //...
    return 0;
}

// v5: concept as template argument
template <std::integral T>    // std::integral is a concept
int count_one_bits(T arg)
{
    //...
    return 0;
}

// v6: auto with concept
int count_one_bits1(std::integral auto arg)    // rename to avoid duplication of above one
{
    //...
    return 0;
}

// v7: 'requires' at the end of function declaration
template <typename T>
int count_one_bits(T arg)
    requires std::is_integral_v<T>
{
    //...
    return 0;
}

//
// specialization
//
// v1: specialization all cases that don't fit for sizeof(..)
template <typename T>
struct safe_sizeof {
    static constexpr std::size_t value = sizeof(T);
};

template <>
struct safe_sizeof<void> {
    static constexpr std::size_t value = 0;
};

// specialization for array with 0 length
template <typename T>
struct safe_sizeof<T[]> {
    static constexpr std::size_t value = 0;
};

// specialization for function with return type R and arguments Args...
template <typename R, typename... Args>
struct safe_sizeof<R(Args...)> {
    static constexpr std::size_t value = 0;
};

// v2: 'requires' sizeof(..) cases
template <typename T>
struct safe_sizeof1 {
    static constexpr std::size_t value = 0;
};

template <typename T>
    requires(sizeof(T) > 0)
struct safe_sizeof1<T> {
    static constexpr std::size_t value = sizeof(T);
};

// v3: modern way by using variable template
template <typename T>
constexpr std::size_t safe_sizeof2 = 0;

template <typename T>
    requires(sizeof(T) > 0)
constexpr std::size_t safe_sizeof2<T> = sizeof(T);

///
///

// 1) by utilizing pointer to function, detect the constraint at compile time
//
// https://isocpp.org/wiki/faq/templates
// http://www.stroustrup.com/bs_faq2.html#constraints

template <class T, class U>
struct Derived_from {
    static void constraints(T* p) { [[maybe_unused]] U* pb = p; }
    Derived_from() { [[maybe_unused]] void (*p)(T*) = constraints; }
};
template <class T1, class T2>
struct Can_copy {
    static void constraints(T1 a, T2 b)
    {
        T2 c = a;
        b    = a;
    }
    Can_copy() { void (*p)(T1, T2) = constraints; }
};
template <class T1, class T2 = T1>
struct Can_compare {
    static void constraints(T1 a, T2 b)
    {
        a == b;
        a > b;
        a < b;
        a != b;
    }
    Can_compare() { void (*p)(T1, T2) = constraints; }
};
template <class T1, class T2, class T3 = T1>
struct Can_multiply {
    static void constraints(T1 a, T2 b, T3 c) { c = a * b; }
    Can_multiply() { void (*p)(T1, T2, T3) = constraints; }
};

struct B_ {
};
struct D : B_ {
};
struct DD : D {
};
struct X {
};

// class template
template <class T>
class Container : Derived_from<T, B_>
{
    // ...
};
// function template
template <class T>
void RunDerived(const T& o)
{
    // accept object of only B or derived from B_
    Derived_from<T, B_>();

    const B_* pB = &o;
    std::cout << sizeof(*pB) << std::endl;
}

void run_template_constraints_1()
{
    Derived_from<D, B_>();
    Derived_from<DD, B_>();
    //    Derived_from<X, B_>(); // compile error
    //    Derived_from<int, B_>(); // compile error
    //    Derived_from<X, int>(); // compile error

    Container<B_> bc;
    Container<D>  dc;
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
template <class T>
class ContainerB
{
    static_assert(std::is_base_of<B_, T>::value, "T must inherit from B_");
    // ...
};

// 'requires'
template <typename T>
    requires std::is_base_of_v<B_, T>
struct ContainerB2 {
};

// function template with constainted type as int
template <class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
void increment(T& x)
{
    x++;
}

// use built-in concept before 'auto'
void increment2(std::integral auto& x)
{
    ++x;
}

// the return type (bool) is only valid if T is integral type
template <class T>
typename std::enable_if<std::is_integral<T>::value, bool>::type is_odd(T i)
{
    return bool(i % 2);
}

// use built-in concept before 'auto'
bool is_odd2(std::integral auto i)
{
    return bool(i % 2);
}

// the second template argument is only valid if T is integral type
template <class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
bool is_even(T i)
{
    return !bool(i % 2);
}

void run_template_constraints_2()
{
    [[maybe_unused]] ContainerB<B_>  b;
    [[maybe_unused]] ContainerB<DD>  dd;
    [[maybe_unused]] ContainerB2<B_> b2;
    [[maybe_unused]] ContainerB2<DD> dd2;
    // ContainerB<X> x; // compile error
    // ContainerB<int> i; // compile error
    // ContainerB2<X> x; // compile error
    // ContainerB2<int> i; // compile error

    [[maybe_unused]] float f = 5.55f;
    int                    a = 10;
    increment(a);
    increment2(a);
    // increment(f); // compile error
    // increment2(f); // compile error
    is_odd(a);
    is_odd2(a);
    // is_odd(f); // compile error
    // is_odd2(f); // compile error
    is_even(a);
    // is_even(f); // compile error
}

