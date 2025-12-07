#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/angry-children
//

int sort_min_fairness(int k, std::vector<int> arr)
{
    std::sort(arr.begin(), arr.end());
    int min_unfairness = arr[k - 1] - arr[0];
    for (unsigned i = k; i < arr.size(); ++i) {
        int unfairness = arr[i] - arr[i - k + 1];
        if (unfairness < min_unfairness) {
            min_unfairness = unfairness;
        }
    }
    return min_unfairness;
}

