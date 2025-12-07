#pragma once

#include "../core/header.h"
#include <type_traits>

//
// SFINAE: Substitution Failure Is Not An Error
//
// https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error
//

// https://www.youtube.com/watch?v=-Z7EOWVkb3M&ab_channel=platis.solutions
struct SFINAE_test {
    void aa(char (*f)[0] = nullptr) { std::cout << "aa: " << sizeof(f) << std::endl; }
    void aa(char (*f)[1] = nullptr) { std::cout << "bb: " << sizeof(f) << std::endl; }

    void run()
    {
        char* ff[0];
        std::cout << sizeof(ff) << std::endl;

        {
            // 0 size array is not allowed by standard, somehow it goes well :))
            char ch[0];
            auto chh = &ch;
            aa(chh);
        }

        {
            char ch[1] = {};
            auto chh   = &ch;
            aa(chh);
        }
    }
};

struct foo {
    using foobar = float;
};

//
// version 1: before c++11
template <typename T>
struct has_typedef_foobar_v1 {
    // Types "yes" and "no" are guaranteed to have different sizes,
    // specifically sizeof(yes) == 1 and sizeof(no) == 2.
    typedef char yes[1];
    typedef char no[2];

    template <typename C>
    static yes& test(typename C::foobar*);

    template <typename>
    static no& test(...);

    // If the "sizeof" of the result of calling test<T>(nullptr) is equal to
    // sizeof(yes), the first overload worked and T has a nested type named
    // foobar.
    static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

void run_template_SFINAE_1()
{
    std::cout << std::boolalpha;
    std::cout << "has_typedef_foobar_v1<int>: " << has_typedef_foobar_v1<int>::value << std::endl;    // Prints false
    std::cout << "has_typedef_foobar_v1<foo>: " << has_typedef_foobar_v1<foo>::value << std::endl;    // Prints true
}

//
// version 2: c++11

// c++11: __cplusplus == 201103L
// c++14: __cplusplus == 201402L
// c++17: __cplusplus == 201703L

// c++17 has provided void_t: https://en.cppreference.com/w/cpp/types/void_t
#if __cplusplus < 201703L
template <typename... ANY>
using void_t = void;
#endif

template <typename T, typename = void>
struct has_typedef_foobar_v2 : std::false_type {
};

template <typename T>
struct has_typedef_foobar_v2<T, std::void_t<typename T::foobar>> : std::true_type {
};

void run_template_SFINAE_2()
{
    std::cout << std::boolalpha;
    std::cout << "has_typedef_foobar_v2<int>: " << has_typedef_foobar_v2<int>::value << std::endl;
    std::cout << "has_typedef_foobar_v2<foo>: " << has_typedef_foobar_v2<foo>::value << std::endl;
}

//
// version 3: detect validity of an expression/method
template <typename T, typename = void>
struct has_pre_increment_member : std::false_type {
};

template <typename T>
struct has_pre_increment_member<T, std::void_t<decltype(++std::declval<T&>())>> : std::true_type {
};

template <typename T, typename = void>
struct is_iterable : std::false_type {
};

template <typename T>
struct is_iterable<
    T, std::void_t<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>>>
    : std::true_type {
};

void run_template_SFINAE_3()
{
    std::cout << std::boolalpha;
    std::cout << "has_pre_increment_member<int>: " << has_pre_increment_member<int>::value << std::endl;
    std::cout << "has_pre_increment_member<foo>: " << has_pre_increment_member<foo>::value << std::endl;
    std::cout << "has_pre_increment_member<std::array<char, 10>>: "
              << has_pre_increment_member<std::array<char, 10>>::value << std::endl;
    std::cout << "has_pre_increment_member<std::array<char, 10>::iterator>: "
              << has_pre_increment_member<std::array<char, 10>::iterator>::value << std::endl;
    std::cout << "has_pre_increment_member<std::vector<int>>: " << has_pre_increment_member<std::vector<int>>::value
              << std::endl;
    std::cout << "has_pre_increment_member<std::vector<int>::iterator>: "
              << has_pre_increment_member<std::vector<int>::iterator>::value << std::endl;

    std::cout << "is_iterable<int>: " << is_iterable<int>::value << std::endl;
    std::cout << "is_iterable<foo>: " << is_iterable<foo>::value << std::endl;
    std::cout << "is_iterable<std::array<char, 10>>: " << is_iterable<std::array<char, 10>>::value << std::endl;
    std::cout << "is_iterable<std::vector<int>>: " << is_iterable<std::vector<int>>::value << std::endl;
}

