#ifndef SORT_BEAUTIFUL_PAIRS_H
#define SORT_BEAUTIFUL_PAIRS_H

#include "../main/header.h"

// 
// https://www.hackerrank.com/challenges/beautiful-pairs/problem
// 
// this question is equal to find the number of equal elements from two arrays
// 

int sort_beautiful_pairs(vector<int> A, vector<int> B)
{
    auto len = A.size();
    sort(A.begin(), A.end());
    sort(B.begin(), B.end());
    int num_beautiful = 0;
    unsigned idx_A = 0;
    unsigned idx_B = 0;
    while (idx_A < len && idx_B < len) {
        if (A[idx_A] == B[idx_B]) {
            ++idx_A; ++idx_B;
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

#endif

