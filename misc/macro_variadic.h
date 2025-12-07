#pragma once

#include "../core/util_formatter.h"

//
// https://codecraft.co/2014/11/25/variadic-macros-tricks/
//

//
// 1)
// Accept any number of args >= N, but expand to just the Nth one. The macro
// that calls us still only supports 4 args, but the set of values we might
// need to return is 1 larger, so we increase N to 6.
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N

// Count how many args are in a variadic macro. We use GCC/Clang's extension
// (https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html) to handle the case
// where ... expands to nothing. We must add a placeholder arg before
// ##__VA_ARGS__ (its value is totally irrelevant, but it's necessary to
// preserve the shifting offset we want). In addition, we must add 0 as a valid
// value to be in the N position.
#define COUNT_VARARGS(...) _GET_NTH_ARG("ignored", ##__VA_ARGS__, 4, 3, 2, 1, 0)

//
// 2) macro overrides
// Define two overrides that can be used by the expansion of our main macro.
#define _MY_CONCAT3(a, b, c) a b c
#define _MY_CONCAT2(a, b) a b

// Define a macro that uses the "paired, sliding arg list" technique to select the appropriate override. You should
// recognize this as similar to the GET_NTH_ARG() macro in previous examples.
#define _GET_OVERRIDE(_1, _2, _3, NAME, ...) NAME

// Define a macro that concats either 3 or 2 strings together.
#define MY_CONCAT(...) _GET_OVERRIDE(__VA_ARGS__, _MY_CONCAT3, _MY_CONCAT2)(__VA_ARGS__)

//
// 3) for-each loop
// Accept any number of args >= N, but expand to just the Nth one. Here, N == 6.
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N

// clang-format off
#define _fn(_call, x) { _call(x); }
// clang-format on

// Define some macros to help us create overrides based on the arity of a for-each-style macro.
#define _fe_0(_call, ...)
#define _fe_1(_call, x) _fn(_call, x)
#define _fe_2(_call, x, ...) _fn(_call, x) _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _fn(_call, x) _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _fn(_call, x) _fe_3(_call, __VA_ARGS__)

/**
 * Provide a for-each construct for variadic macros. Supports up to 4 args.
 *
 * Example usage1:
 *     #define FWD_DECLARE_CLASS(cls) class cls;
 *     CALL_MACRO_X_FOR_EACH(FWD_DECLARE_CLASS, Foo, Bar)
 *
 * Example usage 2:
 *     #define START_NS(ns) namespace ns {
 *     #define END_NS(ns) }
 *     #define MY_NAMESPACES System, Net, Http
 *     CALL_MACRO_X_FOR_EACH(START_NS, MY_NAMESPACES)
 *     typedef foo int;
 *     CALL_MACRO_X_FOR_EACH(END_NS, MY_NAMESPACES)
 */
#define CALL_MACRO_X_FOR_EACH(x, ...) \
    _GET_NTH_ARG("ignored", ##__VA_ARGS__, _fe_4, _fe_3, _fe_2, _fe_1, _fe_0)(x, ##__VA_ARGS__)

void cb(int x)
{
    printf("in the function: %d\n", x);
}

void run_macro_variadic()
{
    FUNC_BEGIN(run_macro_variadic);

    // TODO: why this output 1 than 0????
    printf("zero args: %d\n", COUNT_VARARGS());
    printf("three args: %d\n", COUNT_VARARGS(1, 2, 3));

    printf("3 args: %s\n", MY_CONCAT("a", "b", "c"));
    printf("2 args: %s\n", MY_CONCAT("a", "b"));

    CALL_MACRO_X_FOR_EACH(cb, 12, 13, 15);
    CALL_MACRO_X_FOR_EACH(
        [](int x) {
            printf("in the lambda: %d\n", x);
        },
        112, 123, 135)
}

