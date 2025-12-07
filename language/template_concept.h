#pragma once

#include "../core/header.h"

// requires expression after function signature
template <typename... Args>
void try_concept_1(Args... args)
    requires(std::is_arithmetic_v<Args> && ...)
{
    std::cout << (args + ...) << std::endl;
}

// define a concept with built-in trait
template <typename T>
concept concept_arithmetic_t = std::is_arithmetic_v<T>;

// use concept as part of template argument
template <concept_arithmetic_t... Args>
void try_concept_2(Args... args)
{
    std::cout << (args + ...) << std::endl;
}

// use concept with keyword auto directly
void try_concept_3(concept_arithmetic_t auto... args)
{
    std::cout << (args + ...) << std::endl;
}

// define a concept that type T must have member variable 'value', all following 3 versions work
template <typename T>
concept concept_class_var_t_1 = requires(T t) { t.value; };

template <typename T>
concept concept_class_var_t_2 = requires { std::declval<T>().value; };

template <typename T>
concept concept_class_var_t_3 = requires {
    { T::value };
};

// define a concept that type T must have member function 'int foo()', all following 2 versions work
template <typename T>
concept concept_class_fun_t_1 = requires(T t) {
    { t.foo() } -> std::same_as<int>;
};

template <typename T>
concept concept_class_fun_t_2 = requires {
    { std::declval<T>().foo() } -> std::same_as<int>;
};

// aggregate multi concepts into one
template <typename T>
concept concept_agg_t = concept_class_var_t_1<T> && concept_class_fun_t_1<T>;

void try_concept_class_var(concept_class_var_t_1 auto obj)
{
}
void try_concept_class_fun(concept_class_fun_t_2 auto obj)
{
}
void try_concept_agg(concept_agg_t auto obj)
{
}
struct test_concept_1 {
    std::string value;
    void        foo() {}
};
struct test_concept_2 {
    int value;
    int foo() { return 0; }
};
struct test_concept_3 {
    int  other;
    void goo() {}
};

void run_template_concept()
{
    try_concept_1(1, 2);
    try_concept_1(1, 2.5);
    // try_concept(1, "a");

    try_concept_2(1, 2);
    try_concept_2(1, 2.5);
    // try_concept_2(1, "a");

    try_concept_class_var(test_concept_1{});
    try_concept_class_var(test_concept_2{});
    // try_concept_class_member(test_concept_3{});

    // try_concept_class_fun(test_concept_1{});
    try_concept_class_fun(test_concept_2{});
    //  try_concept_class_member(test_concept_3{});

    // try_concept_class_fun(test_concept_1{});
    try_concept_agg(test_concept_2{});
    //  try_concept_class_member(test_concept_3{});
}
