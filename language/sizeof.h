#ifndef SIZEOF_H
#define SIZEOF_H

#include "../main/header.h"
#include "stlClass.h"

// http://pppboy.blog.163.com/blog/static/30203796201082494026399/

class EmptyClass
{
};
class CharClass
{
    [[maybe_unused]] char ch;
};
class IntClass
{
    [[maybe_unused]] int it;
};
class CharDoubleClass
{
    [[maybe_unused]] char   ch;
    [[maybe_unused]] double db;
};
class DoubleCharClass
{
    [[maybe_unused]] double db;
    [[maybe_unused]] char   ch;
};
struct CharIntShort {
    char  name[5];
    int   num;
    short score;
};
struct IntCharShort {
    int   num;
    char  name[5];
    short score;
};
struct IntShortChar {
    int   num;
    short score;
    char  name[5];
};
struct IntShortIntShort {
    int   n1;
    short s1;
    int   n2;
    short s2;
};
struct IntDoubleInt {
    int    n1;
    double db;
    int    n2;
};

struct RefChar {
    char& value;
};
struct RefInt {
    bool flag;
    int& value;
};
struct RefString {
    bool         flag;
    std::string& value;
};

void run_CheckBasicTypes()
{
    static_assert(sizeof(std::uintptr_t) == 8, "wrong");

    static_assert((0b0110 & ~(0b0110)) == 0, "");

    static_assert(sizeof(EmptyClass) == 1);
    static_assert(sizeof(CharClass) == 1);
    static_assert(sizeof(IntClass) == 4);
    static_assert(sizeof(CharDoubleClass) == 16);
    static_assert(sizeof(DoubleCharClass) == 16);
    static_assert(sizeof(CharIntShort) == 16);
    static_assert(sizeof(IntCharShort) == 12);
    static_assert(sizeof(IntShortChar) == 12);
    static_assert(sizeof(IntShortIntShort) == 16);
    static_assert(sizeof(IntDoubleInt) == 24);
    static_assert(sizeof(RefChar) == 8);
    static_assert(sizeof(RefInt) == 16);
    static_assert(sizeof(RefString) == 16);

    static_assert(sizeof(int) == 4);
    static_assert(sizeof(short) == 2);
    static_assert(sizeof(long) == 8);
    static_assert(sizeof(float) == 4);
    static_assert(sizeof(char) == 1);
    static_assert(sizeof(wchar_t) == 4);
    static_assert(sizeof(unsigned int) == 4);
    static_assert(sizeof(unsigned long) == 8);
    static_assert(sizeof(std::string) == 24);
    static_assert(sizeof(std::wstring) == 24);

    /*
     int64_t has exactly 64 bits. It might not be defined for all platforms.
     int_least64_t is the smallest type with at least 64 bits.
     int_fast64_t is the type that's fastest to process, with at least 64 bits.
    */
    static_assert(sizeof(int8_t) == 1);
    static_assert(sizeof(uint8_t) == 1);
    static_assert(sizeof(uint_fast8_t) == 1);
    static_assert(sizeof(uint_least8_t) == 1);
    static_assert(sizeof(int16_t) == 2);
    static_assert(sizeof(uint16_t) == 2);
    static_assert(sizeof(u_int16_t) == 2);
    static_assert(sizeof(int32_t) == 4);
    static_assert(sizeof(int_fast32_t) == 4);
    static_assert(sizeof(int_least32_t) == 4);
    static_assert(sizeof(uint32_t) == 4);
    static_assert(sizeof(u_int32_t) == 4);
    static_assert(sizeof(uint_least32_t) == 4);
    static_assert(sizeof(int64_t) == 8);
    static_assert(sizeof(uint64_t) == 8);
    static_assert(sizeof(intmax_t) == 8);

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

