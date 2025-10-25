#ifndef FIND_UNIQUE_NUMBER_H
#define FIND_UNIQUE_NUMBER_H

#include "../main/header.h"

//
// Given an array with size 99, elements are from [1,100] and unique. Find the missing number.
//

// Solution 1:
// Calculate sum and subtract all elements

// Solution 2:
// Use bitwise operation
int find_unique_number(const std::vector<int> a)
{
    int ans = 100;
    for (int i = 0; i < 99; i++) {
        ans ^= i + 1;
        ans ^= a[i];
    }
    return ans;
}

// Solution 3:
// If array is sorted, use binary search, time complexity O(log(n))
//
// A similar question: given an array with unique integers in increasing order, return the integer with its value equal
// to its index in the array.

#endif

