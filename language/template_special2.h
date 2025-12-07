#pragma once

#include "../main/header.h"

//
// This tells how to specialize template class's template method.
// https://stackoverflow.com/questions/5512910/explicit-specialization-of-template-class-member-function
// http://en.cppreference.com/w/cpp/language/template_specialization#Members_of_specializations
//

// solution 1): use enable_if
// The ugly thing is that, with all these enable_if's there should be only one
// specialization available for the compiler otherwise disambiguation error will
// arise. That's why the default behaviour "get as T" needs also an enable_if.
template <typename T>
class t_special_1
{
public:
    template <
        typename U, std::enable_if_t<!std::is_same<double, U>::value && !std::is_same<float, U>::value>* = nullptr>
    void get_as()
    {
        std::cout << "get as T" << std::endl;
    }
    template <typename U, std::enable_if_t<std::is_same<double, U>::value>* = nullptr>
    void get_as()
    {
        std::cout << "get as double" << std::endl;
    }
    template <typename U, std::enable_if_t<std::is_same<float, U>::value>* = nullptr>
    void get_as()
    {
        std::cout << "get as float" << std::endl;
    }
};

// solution 2): use overload function
// This version CANNOT handle the case when method template doesn't take
// any input parameter since get_as_impl (template and overload) gets type
// information with input parameter.
template <typename T>
class t_special_2
{
public:
    template <typename U>
    void get_as(U t)
    {
        get_as_impl(t);
    }

private:
    template <typename U>
    void get_as_impl(U)
    {
        std::cout << "get_as_impl<U>(U)" << std::endl;
    }
    void get_as_impl(double) { std::cout << "get_as_impl(double)" << std::endl; }
    void get_as_impl(float) { std::cout << "get_as_impl(float)" << std::endl; }
};
// solution 2): use overload function
// This version CAN handle non-parameter method by using internal dummy template
// - struct type. Idea is the struct type<T>{} contains type information which
// is needed for get_as_impl template and overload functions.
template <typename T>
class t_special_3
{
public:
    template <typename U>
    void get_as()
    {
        get_as_impl(type<U>{});
    }

private:
    template <typename UT>
    struct type {
    };

    template <typename U>
    void get_as_impl(type<U>)
    {
        std::cout << "get_as_impl<U>(type<U>)" << std::endl;
    }
    void get_as_impl(type<double>) { std::cout << "get_as_impl(type<double>)" << std::endl; }
    void get_as_impl(type<float>) { std::cout << "get_as_impl(type<float>)" << std::endl; }
};

void run_template_specialization_2()
{
    // solution 1)
    std::cout << "------------- solution 1) enable_if -------------" << std::endl;
    t_special_1<int> t1_i;
    t1_i.get_as<int>();
    t1_i.get_as<char>();
    t1_i.get_as<double>();
    t1_i.get_as<float>();

    t_special_1<float> t1_f;
    t1_f.get_as<int>();
    t1_f.get_as<char>();
    t1_f.get_as<double>();
    t1_f.get_as<float>();

    // solution 2), with input parameter
    std::cout << "------ solution 2), overload with parameter ------" << std::endl;
    t_special_2<int> t2_i;
    int              n = 10;
    t2_i.get_as<int>(n);
    char c = 'a';
    t2_i.get_as<char>(c);
    double o = 1.1;
    t2_i.get_as<double>(o);
    float f = 1.5;
    t2_i.get_as<float>(f);

    t_special_2<float> t2_f;
    t2_f.get_as<int>(n);
    t2_f.get_as<char>(c);
    t2_f.get_as<double>(o);
    t2_f.get_as<float>(f);

    // solution 2), without input parameter
    std::cout << "----- solution 2), overload without parameter ----" << std::endl;
    t_special_3<float> t3_f;
    t3_f.get_as<int>();
    t3_f.get_as<char>();
    t3_f.get_as<double>();
    t3_f.get_as<float>();
}
