#ifndef TYPE_DEDUCTION_H
#define TYPE_DEDUCTION_H

#include "../main/header.h"
#include "../main/utility.h"

int g_arr[] = {1,2,3};
int* get_i() {
    //  static int g_i = 10;
    return g_arr;
}
#define CHECK_BODY(t)                                                                                                                           \
do {                                                                                                                                            \
    cout << setw(12) << left << "sizeof(T):"    << sizeof(T)    << endl;                                                                        \
    cout << setw(12) << left << "sizeof(T&&):"  << sizeof(T&&)  << endl;                                                                        \
    cout << setw(12) << left << "sizeof(t):"    << sizeof(t)    << endl;                                                                        \
    cout << "decltype(T&&) " << (is_same<T&&, decltype(t)>::value ? "==" : "!=") << " decltype(t)" << endl;                                     \
    cout << setw(12) << left << ""              << setw(6) << "T"                           << "t"                                      << endl;\
    cout << setw(12) << left << "int: "         << setw(6) << is_same<T, int>::value        << is_same<decltype(t), int>::value         << endl;\
    cout << setw(12) << left << "int*: "        << setw(6) << is_same<T, int*>::value       << is_same<decltype(t), int*>::value        << endl;\
    cout << setw(12) << left << "int&: "        << setw(6) << is_same<T, int&>::value       << is_same<decltype(t), int&>::value        << endl;\
    cout << setw(12) << left << "int*&: "       << setw(6) << is_same<T, int*&>::value      << is_same<decltype(t), int*&>::value       << endl;\
    cout << setw(12) << left << "int&&: "       << setw(6) << is_same<T, int&&>::value      << is_same<decltype(t), int&&>::value       << endl;\
    cout << setw(12) << left << "pointer: "     << setw(6) << is_pointer<T>::value          << is_pointer<decltype(t)>::value           << endl;\
    cout << setw(12) << left << "refernce: "    << setw(6) << is_reference<T>::value        << is_reference<decltype(t)>::value         << endl;\
    cout << setw(12) << left << "lvalue_ref: "  << setw(6) << is_lvalue_reference<T>::value << is_lvalue_reference<decltype(t)>::value  << endl;\
    cout << setw(12) << left << "rvalue_ref: "  << setw(6) << is_rvalue_reference<T>::value << is_rvalue_reference<decltype(t)>::value  << endl;\
} while (0)

template <typename T>
void check_plain(T t) {
    CHECK_BODY(t);
}
template <typename T>
void check_pointer(T* t) {
    CHECK_BODY(t);
}
template <typename T>
void check_reference(T& t) {
    CHECK_BODY(t);
}
template <typename T>
void check_universal(T&& t) {
    CHECK_BODY(t);
}

void run_type_deduction()
{
    cout << boolalpha;

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
    cout << '\n';

    // 2) check_pointer
    FUNC_BEGIN(check_pointer);

    SECTION_BEGIN(check_pointer(auto ag = get_i()));
    check_pointer(ag);

    SECTION_BEGIN(check_pointer(get_i()));
    check_pointer(get_i());

    SECTION_BEGIN(check_pointer(auto bg = get_i()));
    check_pointer(bg);
    cout << '\n';

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

#endif

