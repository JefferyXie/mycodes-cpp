#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/beautiful-pairs/problem
//
// this question is equal to find the number of equal elements from two arrays
//

int sort_beautiful_pairs(std::vector<int> A, std::vector<int> B)
{
    auto len = (int)A.size();
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());
    int num_beautiful = 0;
    int idx_A         = 0;
    int idx_B         = 0;
    while (idx_A < len && idx_B < len) {
        if (A[idx_A] == B[idx_B]) {
            ++idx_A;
            ++idx_B;
            ++num_beautiful;
        } else if (A[idx_A] > B[idx_B]) {
            ++idx_B;
        } else {
            ++idx_A;
        }
    }
    // you have to change 1 element no matter whatever case it is
    return (num_beautiful == len) ? --len : ++num_beautiful;
}

