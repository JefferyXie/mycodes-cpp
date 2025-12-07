#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/largest-permutation/problem
//

std::vector<int> array_largest_permutation(int k, std::vector<int> arr)
{
    const auto            len = arr.size();
    std::vector<unsigned> idx(len + 1);
    for (unsigned i = 0; i < len; ++i) {
        idx[arr[i]] = i;
    }
    for (size_t i = 0; i < len && k > 0; ++i) {
        if (arr[i] == static_cast<int>(len - i))
            continue;

        arr[idx[len - i]] = arr[i];
        idx[arr[i]]       = idx[len - i];

        arr[i]       = len - i;
        idx[len - i] = i;

        --k;
    }
    return arr;
}

