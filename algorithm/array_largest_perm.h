#ifndef ARRAY_LARGEST_PERM_H
#define ARRAY_LARGEST_PERM_H

#include "../main/header.h"

// 
// https://www.hackerrank.com/challenges/largest-permutation/problem
// 

vector<int>
array_largest_permutation(int k, vector<int> arr)
{
    auto len = arr.size();
    vector<unsigned> idx(len+1);
    for (unsigned i = 0; i < len; ++i) {
        idx[arr[i]] = i;
    }
    for (size_t i = 0; i < len && k > 0; ++i) {
        if (arr[i] == len-i) continue;

        arr[idx[len-i]] = arr[i];
        idx[arr[i]] = idx[len - i];

        arr[i] = len - i;
        idx[len - i] = i;
        
        --k;
    }
    return arr;
}

#endif
