#pragma once

#include "../main/header.h"

//
// https://www.prodevelopertutorial.com/add-and-subtract-2-numbers-using-bitwise-operators-c-solution/
//
int bitwise_addition(int a, int b)
{
    while (b) {
        int carry = a & b;
        a         = a ^ b;
        b         = carry << 1;
    }
    return a;
}

int bitwise_subtraction(int a, int b)
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
int bitwise_multiplication(int a, int b)
{
    int res = 0;
    while (b) {
        if (b & 1)
            res += a;

        // double a since b is halved
        a = a << 1;
        b = b >> 1;
    }
    return res;
}

//
// http://stackoverflow.com/questions/5386377/division-without-using
//
int bitwise_division(int dividend, int divisor)
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

