#ifndef SUBARRAY_MIN_COUNT_SUM_H
#define SUBARRAY_MIN_COUNT_SUM_H

#include "../main/header.h"

//
// https://www.hackerrank.com/challenges/subset-sum/problem

//
// dynamic programming
int subarray_min_count_sum(const vector<int>& arr, int sum)
{
    auto                len = arr.size();
    vector<vector<int>> dp;
    dp.resize(len, vector<int>(sum + 1, 0));
    for (int j = 0; j < sum + 1; ++j) {
        dp[0][j] = (arr[0] >= j ? 1 : -1);
    }
    for (size_t i = 1; i < len; ++i) {
        for (int j = 0; j < sum + 1; ++j) {
            if (arr[i] >= j) {
                dp[i][j] = 1;
            } else {
                int v = 0;
                // doesn't include arr[i] into sum
                auto v1 = dp[i - 1][j];    // -1, or > 0
                // include arr[i] into sum
                auto v2 = dp[i - 1][j - arr[i]] + 1;    // 0, or > 0
                if (v1 > 0) {
                    if (v2 > 0) {
                        v = min(v1, v2);
                    } else {
                        v = v1;
                    }
                } else {
                    if (v2 > 0) {
                        v = v2;
                    } else {
                        v = -1;
                    }
                }
                dp[i][j] = v;
            }
        }
    }
    return dp[len - 1][sum];
}

#endif
