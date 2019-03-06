#ifndef SUBARRAY_MAX_NO_SKIP_H
#define SUBARRAY_MAX_NO_SKIP_H

#include "../main/header.h"

// 
/*
 * http://stackoverflow.com/questions/29236837/find-max-sum-of-elements-in-an-array-with-twist/42330440
 * Find the maximum sum such that you are not allowed to skip 2 contiguous elements 
 * 
 * Examples :
 * 
 * Input : arr[] = {10, 20, 30, -10, -50, 40, -50, -1, -3}
 * Output : 10+20+30-10+40-1 = 89
 * 
 */

// Use a recurrence that accounts for that:
// dp[-1] = 0, dp[0] = a[0]. 
// dp[i] = max(
//             dp[i - 1] + a[i], <- take two consecutives
//             dp[i - 2] + a[i], <- skip a[i - 1]
//            )
 
int subarray_max_no_skip(int arr[], int n)
{
    // TODO...
    return 0;
}

#endif

