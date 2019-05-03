#ifndef STR_PALINDROME_H
#define STR_PALINDROME_H

#include "../main/header.h"

// 
// https://www.hackerrank.com/challenges/game-of-thrones
// 

// this practice gives a good example to count characters by using integer
// (bits) than array
string
str_palindrome(string s)
{
    //Track even-ness of letters
    int bits = 0;
    //For every character in the input string
    //Get the character's ASCII code and update bits
    for (auto c : s) {
        bits ^= (1 << int(c - 'a'));
    }
    
    //Check if palindrome:
    //   - Iff str length is even, no letter can appear an odd amount of times
    //   - Iff str length is odd, one letter must appear an odd amount of times
    return (bits & (bits - (int)(s.size() & 1))) ? "NO" : "YES";
}

#endif
