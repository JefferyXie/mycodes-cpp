#pragma once

#include "../core/header.h"

// https://www.hackerrank.com/challenges/missing-numbers
std::set<int> find_missing_numbers(int* A, int n, int* B, int m)
{
    std::set<int> missingSet;
    int           min_B = *B;
    int           max_B = *B;
    for (int i = 1; i < m; ++i) {
        min_B = std::min(*(B + i), min_B);
        max_B = std::max(*(B + i), max_B);
    }
    int existing[max_B - min_B + 1];
    std::memset(existing, 0, sizeof(existing));
    for (int i = 0; i < m; ++i) {
        int index = *(B + i) - min_B;
        existing[index]++;
    }
    for (int i = 0; i < n; ++i) {
        int a = *(A + i);
        if (a > max_B || a < min_B)
            missingSet.insert(a);
        else
            --existing[a - min_B];
    }
    for (unsigned int i = 0; i < sizeof(existing) / sizeof(existing[0]); ++i) {
        if (existing[i] != 0)
            missingSet.insert(i + min_B);
    }
    return missingSet;
}

