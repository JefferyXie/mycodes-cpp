#ifndef SIZEOF_H
#define SIZEOF_H

#include "../main/header.h"
#include "stlClass.h"

// http://pppboy.blog.163.com/blog/static/30203796201082494026399/

class EmptyClass { };
class CharClass
{
    char m_ch;
};
class IntClass
{
    int m_int;
};
class CharDoubleClass
{
    char m_ch;
    double m_db;
};
class DoubleCharClass
{
    double m_db;
    char m_ch;
};
struct CharIntShort
{
    char name[5];
    int num;
    short score;
};
struct IntCharShort
{
    int num;
    char name[5];
    short score;
};
struct IntShortChar
{
    int num;
    short score;
    char name[5];
};
struct IntShortIntShort
{
    int n1;
    short s1;
    int n2;
    short s2;
};

template<typename T> void
run_CheckClass(T t) {
    cout << "sizeof(T): " << sizeof(t) << endl;
    cout << "sizeof(EmptyClass): " << sizeof(EmptyClass) << endl;
    cout << "sizeof(CharClass): " << sizeof(CharClass) << endl;
    cout << "sizeof(IntClass): " << sizeof(IntClass) << endl;
    cout << "sizeof(CharDoubleClass): " << sizeof(CharDoubleClass) << endl;
    cout << "sizeof(DoubleCharClass): " << sizeof(DoubleCharClass) << endl;
    cout << "sizeof(CharIntShort): " << sizeof(CharIntShort) << endl;
    cout << "sizeof(IntCharShort): " << sizeof(IntCharShort) << endl;
    cout << "sizeof(IntShortChar): " << sizeof(IntShortChar) << endl;
    cout << "sizeof(IntShortIntShort): " << sizeof(IntShortIntShort) << endl;
    cout << "sizeof(stlClass): " << sizeof(stlClass) << endl;
}

void
run_CheckBasicTypes() {
    cout << "sizeof(int): " << sizeof(int) <<endl;
    cout << "sizeof(short): " << sizeof(short) << endl;
    cout << "sizeof(long): " << sizeof(long) << endl;
    cout << "sizeof(float): " << sizeof(float) << endl;
    cout << "sizeof(char): " << sizeof(char) << endl;
    cout << "sizeof(wchar_t): " << sizeof(wchar_t) << endl;
    cout << "sizeof(unsigned int): " << sizeof(unsigned int) << endl;
    cout << "sizeof(unsigned long): " << sizeof(unsigned long) << endl;
    cout << "sizeof(string): " << sizeof(string) << endl;
    cout << "sizeof(wstring): " << sizeof(wstring) << endl;

    /*
     int64_t has exactly 64 bits. It might not be defined for all platforms.
     int_least64_t is the smallest type with at least 64 bits.
     int_fast64_t is the type that's fastest to process, with at least 64 bits.
    */
    cout << "sizeof(int8_t): " << sizeof(int8_t) << endl;
    cout << "sizeof(uint8_t): " << sizeof(uint8_t) << endl;
    cout << "sizeof(uint_fast8_t): " << sizeof(uint_fast8_t) << endl;
    cout << "sizeof(uint_least8_t): " << sizeof(uint_least8_t) << endl;
    cout << "sizeof(int16_t): " << sizeof(int16_t) << endl;
    cout << "sizeof(uint16_t): " << sizeof(uint16_t) << endl;
    cout << "sizeof(u_int16_t): " << sizeof(u_int16_t) << endl;
    cout << "sizeof(int32_t): " << sizeof(int32_t) << endl;
    cout << "sizeof(int_fast32_t): " << sizeof(int_fast32_t) << endl;
    cout << "sizeof(int_least32_t): " << sizeof(int_least32_t) << endl;
    cout << "sizeof(uint32_t): " << sizeof(uint32_t) << endl;
    cout << "sizeof(u_int32_t): " << sizeof(u_int32_t) << endl;
    cout << "sizeof(uint_least32_t): " << sizeof(uint_least32_t) << endl;
    cout << "sizeof(int64_t): " << sizeof(int64_t) << endl;
    cout << "sizeof(uint64_t): " << sizeof(uint64_t) << endl;
    cout << "sizeof(intmax_t): " << sizeof(intmax_t) << endl;

    // C++ raw string literals
    string test = R"(c:\folder\file1.txt)";
    cout << test << endl;
    test = R"(1st line.\n2nd line.\n3rd line.)";
    cout << test << endl;
    test =
        R"(1st line.
        2nd line.
        3rd line.)";
    cout << test << endl;
    test = R"(1st line.
    2nd line.
    3rd line.)";
    cout << test << endl;
}

#endif

