#ifndef SUBARRAY_MAX_NO_ADJACENT_H
#define SUBARRAY_MAX_NO_ADJACENT_H

#include "../main/header.h"

// 
/*
 * http://www.geeksforgeeks.org/maximum-sum-such-that-no-two-elements-are-adjacent/
 * Maximum sum such that no two elements are adjacent
 *
 * Examples :
 * Input : arr[] = {5, 5, 10, 100, 10, 5}
 * Output : 110
 * 
 * Input : arr[] = {1, 2, 3}
 * Output : 4
 * 
 * Input : arr[] = {1, 20, 3}
 * Output : 20
 */

// 
// solution 1), recursive version, doing a lot of duplicate call & calc
long subarray_max_non_adjacent_recursive(const vector<int>& arr,
        unsigned int index, bool index_picked, long max_by_far)
{
    if (index == 0) return max_by_far;

    if (index_picked) {
        return subarray_max_non_adjacent_recursive(arr, index - 1, false, max_by_far);
    } else {
        return max(subarray_max_non_adjacent_recursive(arr, index - 1, false, max_by_far),
                   subarray_max_non_adjacent_recursive(arr, index - 1, true, max_by_far + arr[index - 1]));
    }
}

//
// solution 2), using dynamic programming, keep intermediate results
long subarray_max_non_adjacent_dp(const vector<int>& arr)
{
    auto len = arr.size();
	// dp[0]: the max subset sum in range arr[0, i], given arr[i] is not selected
	// dp[1]: the max subset sum in range arr[0, i], given arr[i] is selected
    vector<long> dp[2];
    dp[0].resize(len);
    dp[1].resize(len);
    dp[0][0] = 0;
    dp[1][0] = arr[0];

    for (unsigned int i = 1; i < len; ++i) {
        dp[0][i] = max(dp[0][i-1], dp[1][i-1]);
        dp[1][i] = dp[0][i-1] + arr[i];
    }
    return max(dp[0][len-1], dp[1][len-1]);
}

//
// solution 3), another way by using dynamic programming
long subarray_max_non_adjacent_dp_2(const vector<int>& arr)
{
    auto len = arr.size();
    if (len == 0) return 0;
    if (len == 1) return arr[0];

    auto vec = arr;
    vec[1] = max(vec[0], vec[1]);
    
    for (unsigned int i = 2; i < len; ++i) {
		// 3 cases here
		vec[i] = max(vec[i],
					 max(vec[i] + vec[i-2],
						 vec[i-1]));
   }
    return vec[len-1];
}

//
// solution 4), still dynamic programming but without array cache
// 1) Loop for all elements in arr[] and maintain two sums incl and excl where incl = Max
//    sum including the previous element and excl = Max sum excluding the previous element.
// 2) Max sum excluding the current element will be max(incl, excl) and max sum including
//    the current element will be excl + current element (Note that only excl is considered
//    because elements cannot be adjacent).
// 3) At the end of the loop return max of incl and excl.
long subarray_max_non_adjacent_dp_3(const vector<int>& arr)
{
    auto n = arr.size();
    int incl = arr[0];
    int excl = 0;
    int excl_new;
    for (int i = 1; i < n; i++)
    {
        /* current max excluding i */
        excl_new = incl > excl ? incl: excl;

        /* current max including i */
        incl = excl + arr[i];
        excl = excl_new;
    }

    /* return max of incl and excl */
    return incl > excl ? incl : excl;
}

#endif

