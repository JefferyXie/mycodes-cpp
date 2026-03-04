#pragma once

#include "../core/header.h"
#include <type_traits>

//
// https://www.prodevelopertutorial.com/add-and-subtract-2-numbers-using-bitwise-operators-c-solution/
//
constexpr int bitwise_addition(int a, int b)
{
    while (b) {
        int carry = a & b;
        a         = a ^ b;
        b         = carry << 1;
    }
    return a;
}

constexpr int bitwise_subtraction(int a, int b)
{
    while (b) {
        int borrow = (~a) & b;
        a          = a ^ b;
        b          = borrow << 1;
    }
    return a;
}

//
// http://www.geeksforgeeks.org/russian-peasant-multiply-two-numbers-using-bitwise-operators/
//
constexpr int bitwise_multiplication(int a, int b)
{
    int res = 0;
    while (b) {
        if (b & 1) {
            res += a;
        }

        // double a since b is halved
        a = a << 1;
        b = b >> 1;
    }
    return res;
}

//
// http://stackoverflow.com/questions/5386377/division-without-using
//
// TODO: support negative values...
constexpr int bitwise_division(int dividend, int divisor)
{
    int denom   = divisor;
    int current = 1;
    int answer  = 0;

    if (denom > dividend)
        return 0;

    if (denom == dividend)
        return 1;

    while (denom <= dividend) {
        denom   = denom << 1;
        current = current << 1;
    }

    denom   = denom >> 1;
    current = current >> 1;

    while (current != 0) {
        // this is bit operation, so one time comparation is enough
        if (dividend >= denom) {
            dividend -= denom;
            answer |= current;
        }
        current = current >> 1;
        denom   = denom >> 1;
    }
    return answer;
}

template <typename T>
    requires std::is_integral_v<T> && std::is_signed_v<T>
constexpr T max_value(T a, T b)
{
    constexpr auto ShiftBits   = (sizeof(T) * 8 - 1);
    const auto     diff        = a - b;
    const auto     is_negative = (diff >> ShiftBits) & 0x01;
    return a - is_negative * diff;
    // or,
    // ((a + b) + is_negative * (a - b)) / 2;
}

template <typename T>
    requires std::is_integral_v<T> && std::is_unsigned_v<T>
constexpr T max_value(T a, T b)
{
    // TODO: is there a better way without using signed type here?
    using SignedT              = std::make_signed_t<T>;
    constexpr auto ShiftBits   = (sizeof(SignedT) * 8 - 1);
    const auto     diff        = static_cast<SignedT>(a - b);
    const auto     is_negative = (diff >> ShiftBits) & 0x01;
    return a - is_negative * diff;
    // or,
    // ((a + b) + is_negative * (a - b)) / 2;
}

void run_bitwise_operation()
{
    static_assert(bitwise_addition(1, 2) == 3);
    static_assert(bitwise_addition(-1, 2) == 1);
    static_assert(bitwise_addition(-2, -2) == -4);

    static_assert(bitwise_subtraction(1, 2) == -1);
    static_assert(bitwise_subtraction(2, 1) == 1);
    static_assert(bitwise_subtraction(-2, -1) == -1);

    static_assert(bitwise_multiplication(3, 5) == 15);
    static_assert(bitwise_multiplication(-11, 11) == -121);

    static_assert(bitwise_division(10, 1) == 10);
    static_assert(bitwise_division(10, 2) == 5);
    static_assert(bitwise_division(10, 3) == 3);
    static_assert(bitwise_division(10, 4) == 2);
    // TODO: not yet support negative values
    // static_assert(bitwise_division(10, -3) == -3);

    static_assert(max_value(1, 2) == 2);
    static_assert(max_value(2, 1) == 2);
    static_assert(max_value(-2, 1) == 1);
    static_assert(max_value(-2, -1) == -1);

    static_assert(max_value(10u, 20u) == 20);
    static_assert(max_value(20u, 10u) == 20);
}
