#ifndef SUBARRAY_MAX_SUM_H
#define SUBARRAY_MAX_SUM_H

#include "../main/header.h"

// 
// https://www.hackerrank.com/challenges/maxsubarray
// 

// solution 1), keep all steps' values, not space efficient
vector<int> subarray_max_sum_1(vector<int> arr)
{
    auto len = arr.size();
    vector<int> dp[3];
    dp[0].resize(len, numeric_limits<int>::min());
    dp[1].resize(len, numeric_limits<int>::min());
    dp[2].resize(len, numeric_limits<int>::min());
    dp[0][0] = numeric_limits<int>::min();
    dp[1][0] = arr[0];
    dp[2][0] = arr[0];
    for (unsigned i = 1; i < len; ++i) {
        dp[0][i] = max(dp[0][i-1], dp[1][i-1]);
        dp[1][i] = max(arr[i], arr[i] + dp[1][i-1]);

        dp[2][i] = max(max(arr[i], arr[i]+dp[2][i-1]),
                       dp[2][i-1]);
    }
    auto max_subarr = max(dp[0][len-1], dp[1][len-1]);
    auto max_subseq = dp[2][len-1];
    return {max_subarr, max_subseq};
}

// solution 2), just keep latest snap values
vector<int> subarray_max_sum_2(vector<int> arr)
{
    auto len = arr.size();
    int max_subarr_so_far = arr[0];
    int max_subarr_ending = arr[0];
    int max_subsequence   = arr[0];
    for (unsigned i = 1; i < len; ++i) {
        max_subarr_ending = max(max_subarr_ending + arr[i], arr[i]);
        max_subarr_so_far = max(max_subarr_so_far, max_subarr_ending);
        max_subsequence = max(max(max_subsequence, max_subsequence + arr[i]), arr[i]);
    }
    return {max(max_subarr_so_far, max_subarr_ending), max_subsequence};
}

#endif

