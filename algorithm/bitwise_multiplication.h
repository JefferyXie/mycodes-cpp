#ifndef BITWISE_MULTIPLICATION_H
#define BITWISE_MULTIPLICATION_H

#include "../main/header.h"

// 
// http://www.geeksforgeeks.org/russian-peasant-multiply-two-numbers-using-bitwise-operators/
// Multiply two numbers without multiplication or division operators (using bitwise operators)
// 
// Let the two given numbers be 'a' and 'b'
// 1) Initialize result 'res' as 0.
// 2) Do following while 'b' is greater than 0
//    a) If 'b' is odd, add 'a' to 'res'
//    b) Double 'a' and halve 'b'
// 3) Return 'res'.
unsigned int bitwise_multiplication(unsigned int a, unsigned int b)
{
    int res = 0;  // initialize result

    // While second number doesn't become 1
    while (b > 0)
    {
        // If second number becomes odd, add the first number to result
        if (b & 1)
            res = res + a;

        // Double the first number and halve the second number
        a = a << 1;
        b = b >> 1;
    }
    return res;
}

#endif

