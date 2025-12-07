#pragma once

#include "../core/util_formatter.h"
#include "../core/util.h"

int  g_arr[] = {1, 2, 3};
int* get_i()
{
    //  static int g_i = 10;
    return g_arr;
}

// clang-format off
#define CHECK_BODY(t)                                                                                                                           \
do {                                                                                                                                            \
    std::cout << std::setw(12) << std::left << "sizeof(T):"    << sizeof(T)    << std::endl;                                                    \
    std::cout << std::setw(12) << std::left << "sizeof(T&&):"  << sizeof(T&&)  << std::endl;                                                    \
    std::cout << std::setw(12) << std::left << "sizeof(t):"    << sizeof(t)    << std::endl;                                                    \
    std::cout << "decltype(T&&) " << (std::is_same<T&&, decltype(t)>::value ? "==" : "!=") << " decltype(t)" << std::endl;                      \
    std::cout << std::setw(12) << std::left << ""              << std::setw(6) << "T"                                << "t"                                           << std::endl;\
    std::cout << std::setw(12) << std::left << "int: "         << std::setw(6) << std::is_same<T, int>::value        << std::is_same<decltype(t), int>::value         << std::endl;\
    std::cout << std::setw(12) << std::left << "int*: "        << std::setw(6) << std::is_same<T, int*>::value       << std::is_same<decltype(t), int*>::value        << std::endl;\
    std::cout << std::setw(12) << std::left << "int&: "        << std::setw(6) << std::is_same<T, int&>::value       << std::is_same<decltype(t), int&>::value        << std::endl;\
    std::cout << std::setw(12) << std::left << "int*&: "       << std::setw(6) << std::is_same<T, int*&>::value      << std::is_same<decltype(t), int*&>::value       << std::endl;\
    std::cout << std::setw(12) << std::left << "int&&: "       << std::setw(6) << std::is_same<T, int&&>::value      << std::is_same<decltype(t), int&&>::value       << std::endl;\
    std::cout << std::setw(12) << std::left << "pointer: "     << std::setw(6) << std::is_pointer<T>::value          << std::is_pointer<decltype(t)>::value           << std::endl;\
    std::cout << std::setw(12) << std::left << "refernce: "    << std::setw(6) << std::is_reference<T>::value        << std::is_reference<decltype(t)>::value         << std::endl;\
    std::cout << std::setw(12) << std::left << "lvalue_ref: "  << std::setw(6) << std::is_lvalue_reference<T>::value << std::is_lvalue_reference<decltype(t)>::value  << std::endl;\
    std::cout << std::setw(12) << std::left << "rvalue_ref: "  << std::setw(6) << std::is_rvalue_reference<T>::value << std::is_rvalue_reference<decltype(t)>::value  << std::endl;\
} while (0)
// clang-format on

template <typename T>
void check_plain(T t)
{
    CHECK_BODY(t);
}
template <typename T>
void check_pointer(T* t)
{
    CHECK_BODY(t);
}
template <typename T>
void check_reference(T& t)
{
    CHECK_BODY(t);
}
template <typename T>
void check_universal(T&& t)
{
    CHECK_BODY(t);
}

void run_type_deduction()
{
    std::cout << std::boolalpha;

    // 1) check_plain
    FUNC_BEGIN(check_plain);

    SECTION_BEGIN(check_plain(auto ag = get_i()));
    auto ag = get_i();
    check_plain(ag);

    SECTION_BEGIN(check_plain(get_i()));
    check_plain(get_i());

    SECTION_BEGIN(check_plain(*get_i()));
    check_plain(*get_i());

    auto bg = get_i();
    SECTION_BEGIN(check_plain(auto bg = get_i()));
    check_plain(bg);
    std::cout << '\n';

    // 2) check_pointer
    FUNC_BEGIN(check_pointer);

    SECTION_BEGIN(check_pointer(auto ag = get_i()));
    check_pointer(ag);

    SECTION_BEGIN(check_pointer(get_i()));
    check_pointer(get_i());

    SECTION_BEGIN(check_pointer(auto bg = get_i()));
    check_pointer(bg);
    std::cout << '\n';

    // 3) check_universal
    FUNC_BEGIN(check_universal);

    SECTION_BEGIN(check_universal(auto ag = get_i()));
    check_universal(ag);

    SECTION_BEGIN(check_universal(get_i()));
    check_universal(get_i());

    SECTION_BEGIN(check_universal(*get_i()));
    check_universal(*get_i());

    SECTION_BEGIN(check_universal(auto bg = get_i()));
    check_universal(bg);
}

