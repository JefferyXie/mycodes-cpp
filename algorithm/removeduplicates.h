#ifndef REMOVEDUPLICATES_H
#define REMOVEDUPLICATES_H

#include "../main/header.h"

// http://stackoverflow.com/questions/1532819/algorithm-efficient-way-to-remove-duplicate-integers-from-an-array
// time complexity O(n^2), space complexity O(1)
// 1) loop each element in the array
// 2) for an element, loop the rest of array see it there're duplicates
// if yes, replace its value with last element in the array, and move last pointer to left by one
// 3) the return number is the number of unique elements in the array
int RemoveDuplicates(int* arr, int length) {
    int* last = arr + length - 1;
    for (int* current = arr+1; current <= last; arr++, current = arr+1) {
        while (current <= last) {
            if (*current == *arr) {
                *current = *last;
                last--;
                length--;
            } else {
                current++;
            }
        }
    }
    return length;
}

#endif


