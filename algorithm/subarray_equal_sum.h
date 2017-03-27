#ifndef SUBARRAY_EQUAL_SUM_H
#define SUBARRAY_EQUAL_SUM_H

#include "../main/header.h"

// 
// http://www.geeksforgeeks.org/dynamic-programming-subset-sum-problem/
/*
 Given a set of non-negative integers, and a value sum, determine if there is a subset of the
 given set with sum equal to given sum.

 Examples: set[] = {3, 34, 4, 12, 5, 2}, sum = 9
 Output:  True  //There is a subset (4, 5) with sum 9.
*/

// solution 1):
// 
// Recursive
// Overall: O(2^N)
void subarray_equal_sum_1(const std::vector<int>& A, int currSum, int index, int sum,
                          std::vector<int>& solution)
{
    if (index == (int)A.size()) return;

    if (currSum == sum) {
        std::cout << "Sum found" << std::endl;
        for (int i = 0; i < (int)solution.size(); i++) {
            if (solution[i] == 1) {
                std::cout << " " << A[i];
            }
        }
    } else {
        // include A[index] element into the sum
        solution[index] = 1;
        currSum += A[index];
        subarray_equal_sum_1(A, currSum, index + 1, sum, solution);

        // exclude A[index] element from the sum
        solution[index] = 0;
        currSum -= A[index];	
        subarray_equal_sum_1(A, currSum, index + 1, sum, solution);
    }
}

// solution 2):
// 
// Dynamic programming
// 1) We create a boolean 2D table subset[][] and fill it in bottom up manner.
// 2) The value of subset[i][j] will be true if there is a subset of set[0:j-1] with sum
//    equal to i., otherwise false. Finally, we return subset[sum][n]
// 3) Initially, subset[0][0:j] == TRUE since there exists 'empty subset' sum to 0
//    subset[1:i][0] == FALSE since no elements are in the set
// 4) Now for every element in the set, we have 2 options to determine if the subset exists:
//    Doesn't include element itself A[j-1], so subset[i][j] == subset[i][j-1]
//    Include element A[j-1], then subset[i][j] == subset[i-A[j-1]][j-1]
// Overall: O(N^2)
// 
void subarray_equal_sum_2(const std::vector<int>& A, int sum)
{
    int n = (int)A.size();
    // The value of subset[i][j] will be true if there is a 
    // subset of set[0..j-1] with sum equal to i
    bool subset[sum+1][n+1];
 
    // If sum is 0, then answer is true since we can choose 'empty subset'
    for (int i = 0; i <= n; i++)
    {
        subset[0][i] = true;
    }
 
    // If sum is not 0 and set is empty, then answer is false since no element is available
    for (int i = 1; i <= sum; i++)
    {
        subset[i][0] = false;
    }
 
    // Fill the subset table in botton up manner
    for (int i = 1; i <= sum; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            subset[i][j] = subset[i][j-1];
            if (i >= A[j-1])
            {
                subset[i][j] = subset[i][j-1] || subset[i - A[j-1]][j-1];
            }
        }
    }

    // uncomment this code to print table
    for (int i = 0; i <= sum; i++)
    {
        for (int j = 0; j <= n; j++)
            printf ("%4d", subset[i][j]);
        printf("\n");
    }
 
    std::cout << (subset[sum][n] ? "Sum found" : "Not found") << std::endl;
}

// solution 3):
// 
// Dynamic programming
// http://stackoverflow.com/a/18308020/7169399
// This version even returns the number of sub arrays that sum equal to target
void subarray_equal_sum_3(const std::vector<int>& A, int sum)
{
    int dp[sum + 1];
    dp[0] = 1;
    int currentSum =0;
    for (int i = 0; i < (int)A.size(); i++)
    {
        currentSum += A[i];
        for (int j = std::min(sum, currentSum); j >= A[i]; j--)
        {
            dp[j] += dp[j - A[i]];
        }
    }

    std::cout << "Found the number of subarray: " << dp[sum] << endl;
}

#endif

