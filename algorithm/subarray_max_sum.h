#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/maxsubarray
//
// We define subsequence as any subset of an array. We define a subarray as a contiguous subsequence in an array.
//
// Given an array, find the maximum possible sum among:
// 1) all nonempty subarrays.
// 2) all nonempty subsequences.
// Print the two values as space-separated integers on one line.
//

// solution 1), keep all steps' values, not space efficient
std::vector<int> subarray_max_sum_1(std::vector<int> arr)
{
    const auto                    len = arr.size();
    std::vector<std::vector<int>> dp(3, std::vector<int>(len, std::numeric_limits<int>::min()));
    dp[0][0] = std::numeric_limits<int>::min();
    dp[1][0] = arr[0];
    dp[2][0] = arr[0];
    for (unsigned i = 1; i < len; ++i) {
        dp[0][i] = std::max(dp[0][i - 1], dp[1][i - 1]);
        dp[1][i] = std::max(arr[i], arr[i] + dp[1][i - 1]);

        dp[2][i] = std::max(std::max(arr[i], arr[i] + dp[2][i - 1]), dp[2][i - 1]);
    }
    auto max_subarr = std::max(dp[0][len - 1], dp[1][len - 1]);
    auto max_subseq = dp[2][len - 1];
    return {max_subarr, max_subseq};
}

// solution 2), just keep latest snap values
//
// REMINDER: refer to ./max_increasing_subsequence.h that counts the number of max increasing sub-sequence
//
std::vector<int> subarray_max_sum_2(std::vector<int> arr)
{
    int max_subarr_so_far = arr[0];
    int max_subarr_ending = arr[0];
    int max_subsequence   = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        max_subarr_ending = std::max(max_subarr_ending + arr[i], arr[i]);
        max_subarr_so_far = std::max(max_subarr_so_far, max_subarr_ending);
        max_subsequence   = std::max({max_subsequence, max_subsequence + arr[i], arr[i]});
    }
    return {std::max(max_subarr_so_far, max_subarr_ending), max_subsequence};
}

