#ifndef BITWISE_DIVISION_H
#define BITWISE_DIVISION_H

#include "../main/header.h"

// 
// http://stackoverflow.com/questions/5386377/division-without-using
// Divide two numbers without division operator
// 
unsigned int bitwise_division(unsigned dividend, unsigned divisor) { 
    unsigned int denom   = divisor;
    unsigned int current = 1;
    unsigned int answer  = 0;

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
            answer   |= current;
        }
        current = current >> 1;
        denom   = denom >> 1;
    }    
    return answer;
}

#endif

