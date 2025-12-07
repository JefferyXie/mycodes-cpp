#pragma once

#include "../core/header.h"

//
// Tuple
//
// From: https://gist.github.com/IvanVergiliev/9639530
//
// Another implementation by using meta programming:
// http://blogs.microsoft.co.il/sasha/2015/01/12/implementing-tuple-part-1/
//
// GCC version:
// https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-api-4.5/a01066_source.html
//

//
// my_tuple: definition
//
template <typename First, typename... Rest>
struct my_tuple : public my_tuple<Rest...> {
    my_tuple(First first, Rest... rest) : my_tuple<Rest...>(rest...), first(first) {}

    First first;
};

template <typename First>
struct my_tuple<First> {
    my_tuple(First first) : first(first) {}

    First first;
};

//
// GET_IMPL: meta program that accesses specific element in the tuple
//
template <int index, typename First, typename... Rest>
struct GET_IMPL {
    static auto value(const my_tuple<First, Rest...>* t) -> decltype(GET_IMPL<index - 1, Rest...>::value(t))
    {
        // recursively call/instantiate template<int, typename, typename...> GetImpl
        // the trick here is my_tuple<First, Rest...> inherits from my_tuple<Rest...>
        // so given index, compiler will find the right my_tuple class
        return GET_IMPL<index - 1, Rest...>::value(t);

        // alternatively, find the type of my_tuple<..> given index and args, then do -
        // using Type = Get_Type<index - 1, Rest...>::type;
        // return static_cast<const Type*>(t)->first;
    }
};

template <typename First, typename... Rest>
struct GET_IMPL<0, First, Rest...> {
    static First value(const my_tuple<First, Rest...>* t) { return t->first; }
};

//
// my_get: public interface to access element in the tuple
//
template <int index, typename First, typename... Rest>
auto my_get(const my_tuple<First, Rest...>& t) -> decltype(GET_IMPL<index, First, Rest...>::value(&t))
{
    return GET_IMPL<index, First, Rest...>::value(&t);
}

////////////////////////////////////////////////////////
// Another version of my_tuple_2 by using composition instead of inheritance
template <typename First, typename... Rest>
struct my_tuple_2 {
    my_tuple_2(First first, Rest... rest) : first(first), rest(my_tuple_2<Rest...>(rest...)) {}

    First               first;
    my_tuple_2<Rest...> rest;    // object composition
};

template <typename First>
struct my_tuple_2<First> {
    my_tuple_2(First first) : first(first) {}

    First first;
};

template <int, typename...>
struct GET {
};
template <int Index, typename First, typename... Rest>
struct GET<Index, First, Rest...> {
    static auto value(const my_tuple_2<First, Rest...>& t) { return GET<Index - 1, Rest...>::value(t.rest); }
};
template <typename First, typename... Rest>
struct GET<0, First, Rest...> {
    static auto value(const my_tuple_2<First, Rest...>& t) { return t.first; }
};

template <int Index, typename First, typename... Rest>
auto my_get_2(const my_tuple_2<First, Rest...>& t)
{
    return GET<Index, First, Rest...>::value(t);
}

void run_my_tuple()
{
    {
        my_tuple<double, char, int> c(3.3, 'a', 11);
        printf("%f\n", my_get<0>(c));
        printf("%c\n", my_get<1>(c));
        printf("%d\n", my_get<2>(c));
        //  printf("%d\n", get<3>(c));
    }

    {
        my_tuple<char, char, int, int> c('a', 'b', 22, 11);
        printf("%c\n", my_get<0>(c));
        printf("%c\n", my_get<1>(c));
        printf("%d\n", my_get<2>(c));
        //  printf("%d\n", get<3>(c));
    }

    {
        my_tuple_2<double, char, int> c(3.3, 'a', 11);
        printf("%f\n", my_get_2<0>(c));
        printf("%c\n", my_get_2<1>(c));
        printf("%d\n", my_get_2<2>(c));
    }

    {
        my_tuple_2<double, double, char, int> c(3.3, 2.2, 'a', 11);
        printf("%f\n", my_get_2<0>(c));
        printf("%f\n", my_get_2<1>(c));
        printf("%c\n", my_get_2<2>(c));
        printf("%d\n", my_get_2<3>(c));
    }
}

