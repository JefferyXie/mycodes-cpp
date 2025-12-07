#pragma once

#include "../core/header.h"

// http://stackoverflow.com/questions/1532819/algorithm-efficient-way-to-remove-duplicate-integers-from-an-array
// http://www.geeksforgeeks.org/remove-all-duplicates-from-the-input-string/
//
// Solution 1
// time complexity O(n^2), space complexity O(1)
// 1) loop each element in the array
// 2) for an element, loop the rest of array see it there're duplicates
//    if yes, replace its value with last element in the array, and move last pointer to left by one
// 3) the return number is the number of unique elements in the array
int remove_duplicates_1(int* arr, int length)
{
    int* last = arr + length - 1;
    for (int* current = arr + 1; current <= last; arr++, current = arr + 1) {
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

//
// Solution 2
// O(n*log(n))
// 1) Sort the elements.
// 2) Now in a loop, keep two positions, one to result's index, the other to checking character's index,
//    move checking index further if current character is duplicate
// 3) Remove extra characters at the end of the result string.
char* removeDupsSorted(char* str)
{
    int res_ind = 1, ip_ind = 1;

    /* In place removal of duplicate characters*/
    while (*(str + ip_ind)) {
        if (*(str + ip_ind) != *(str + ip_ind - 1)) {
            *(str + res_ind) = *(str + ip_ind);
            res_ind++;
        }
        ip_ind++;
    }

    /* After above step string is stringiittg.
       Removing extra iittg after string*/
    *(str + res_ind) = '\0';

    return str;
}

char* remove_duplicates_2(char* str)
{
    int n = strlen(str);

    // Sort the character array
    std::sort(str, str + n);

    // Remove duplicates from sorted
    return removeDupsSorted(str);
}

//
// Solution 3
// O(n)
// Use hash (array in this case). Bonus is this solution keeps characters' original order!
char* remove_duplicates_3(char* str)
{
    bool bin_hash[26] = {0};
    int  ip_ind = 0, res_ind = 0;
    /* In place removal of duplicate characters*/
    while (*(str + ip_ind)) {
        char temp = *(str + ip_ind);
        if (bin_hash[(int)temp] == 0) {
            bin_hash[(int)temp] = 1;
            *(str + res_ind)    = *(str + ip_ind);
            res_ind++;
        }
        ip_ind++;
    }
    // After above step string is stringiittg. Removing extra iittg after string
    *(str + res_ind) = '\0';
    return str;
}

// Use STL
// The std::remove_if is O(n) and removes all duplicates to the end. Take a look how
// std::remove_if is implemented, it keeps the left elements in the original order!!
//
// TODO: cleanup the code
void remove_duplicates_4()
{
    std::string              str("Hello World!");
    std::unordered_set<char> log;
    std::cout << "Before: " << str << std::endl;

    str.erase(
        std::remove_if(
            str.begin(), str.end(),
            [&](char const c) {
                return !(log.insert(c).second);
            }),
        str.end());

    std::cout << "After:  " << str << std::endl;
}

