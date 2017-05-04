#ifndef MY_TUPLE_H
#define MY_TUPLE_H

#include "../main/header.h"

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
template<typename First, typename... Rest>
struct my_tuple: public my_tuple<Rest...> {
    my_tuple(First first, Rest... rest): my_tuple<Rest...>(rest...), first(first)
    {}

    First first;
};

template<typename First>
struct my_tuple<First> {
    my_tuple(First first) : first(first)
    {}

    First first;
};

// 
// GET_IMPL: meta program that accesses specific element in the tuple
// 
template<int index, typename First, typename... Rest>
struct GET_IMPL {
    static auto value(const my_tuple<First, Rest...>* t)
        -> decltype(GET_IMPL<index - 1, Rest...>::value(t))
    {
        // recursively call/instantiate template<int, typename, typename...> GetImpl
        return GET_IMPL<index - 1, Rest...>::value(t);
    }
};

template<typename First, typename... Rest>
struct GET_IMPL<0, First, Rest...> {
    static First value(const my_tuple<First, Rest...>* t) {
        return t->first;
    }
};

// 
// my_get: public interface to access element in the tuple
// 
template<int index, typename First, typename... Rest>
auto my_get(const my_tuple<First, Rest...>& t)
        -> decltype(GET_IMPL<index, First, Rest...>::value(&t))
{
    return GET_IMPL<index, First, Rest...>::value(&t);
}

void run_my_tuple() {
  my_tuple<double, char, int> c(3.3, 'a', 11);
  printf("%f\n", my_get<0>(c));
  printf("%c\n", my_get<1>(c));
  printf("%d\n", my_get<2>(c));
//  printf("%d\n", get<3>(c));
}

#endif

