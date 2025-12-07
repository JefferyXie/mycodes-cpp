#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/crush/
// https://www.hackerrank.com/challenges/crush/editorial
//

long array_opera_max(int n, std::vector<std::vector<int>> queries)
{
    std::vector<int> arr(n, 0);
    for (auto& q : queries) {
        int a = q[0];
        int b = q[1];
        int k = q[2];
        // starting point from which k is added to element afterwards
        arr[a - 1] += k;
        // ending point where stops adding k to element
        arr[b] -= k;
    }
    int sum   = 0;
    int max_v = 0;
    for (auto v : arr) {
        // sum here actually gives the final value at particular index
        sum += v;
        max_v = std::max(sum, max_v);
    }
    return max_v;
}

// Segment Tree
// https://www.geeksforgeeks.org/segment-tree-set-1-sum-of-given-range/

