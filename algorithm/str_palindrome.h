#pragma once

#include "../main/header.h"

//
// https://www.hackerrank.com/challenges/game-of-thrones
//

// this practice gives a good example to count characters by using integer (bits) than array
std::string str_palindrome(std::string s)
{
    // Track even-ness of letters
    int bits = 0;
    // For every character in the input string
    // Get the character's ASCII code and update bits
    for (auto c : s) {
        bits ^= (1 << int(c - 'a'));
    }

    // Check if palindrome:
    //    - If str length is even, no letter can appear an odd amount of times
    //    - If str length is odd, one letter must appear an odd amount of times
    return (bits & (bits - (int)(s.size() & 1))) ? "NO" : "YES";
}

// O(n^2)
std::string substring_longest_palindrome(const std::string& str)
{
    auto longest_palindrome = [&str](size_t left, size_t right) {
        while (true) {
            if (str.at(left) == str.at(right)) {
                if (left > 0 && right < str.size() - 1) {
                    --left;
                    ++right;
                } else {
                    return str.substr(left, right - left + 1);
                }
            } else {
                return str.substr(left + 1, right - left - 1);
            }
        }
        return std::string("");
    };

    std::string longest = "";
    for (size_t i = 0; i < str.size() - 1; ++i) {
        auto palindrome_odd  = longest_palindrome(i, i);
        auto palindrome_even = longest_palindrome(i, i + 1);
        if (longest.size() < palindrome_odd.size()) {
            longest = palindrome_odd;
        }
        if (longest.size() < palindrome_even.size()) {
            longest = palindrome_even;
        }
    }
    return longest;
}

