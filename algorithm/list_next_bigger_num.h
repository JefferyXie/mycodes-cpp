#pragma once

#include "../core/header.h"

// 
/*
 * Given an array, for every element, print the bigger one on the right, order doesn't matter
 * 
   Examples :
  
   Input:  Array {1, 3, 2, 4}
   Output: 1->3, 3->4, 2->4
 * 
 */

// time complexity O(n)

// TODO: c++ version
/* python version
def nextGreater(nums):
    stack = []
    res = []
    for i in nums:
        while (len(stack) >= 1 and i > stack[-1]):
            res.append(str(stack[-1]) + "->" + str(i))
            stack.pop(-1)
        stack.append(i)
    return res
*/

