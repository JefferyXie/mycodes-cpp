#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/cut-the-sticks
//

std::vector<int> sort_cut_sticks(std::vector<int> arr)
{
    // arr.assign({1, 13, 3, 8, 14, 9, 4, 4});
    //  8,7,6,4,3,2,1
    std::sort(arr.begin(), arr.end());
    int              len = (int)arr.size();
    std::vector<int> result{len};
    int              shortest = 0;
    int              i        = 0;
    do {
        if (!shortest || shortest == arr[i]) {
            shortest = arr[i];
            ++i;
            continue;
        }
        result.push_back(len - i);
        shortest = 0;

    } while (i < len);
    return result;
}

