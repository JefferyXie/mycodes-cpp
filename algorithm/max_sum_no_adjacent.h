#ifndef MAX_SUM_NO_ADJACENT_H
#define MAX_SUM_NO_ADJACENT_H

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

// 1) Loop for all elements in arr[] and maintain two sums incl and excl where incl = Max
//    sum including the previous element and excl = Max sum excluding the previous element.
// 2) Max sum excluding the current element will be max(incl, excl) and max sum including
//    the current element will be excl + current element (Note that only excl is considered
//    because elements cannot be adjacent).
// 3) At the end of the loop return max of incl and excl.
int max_sum_no_adjacent(int arr[], int n)
{
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

