#ifndef CANDIES_H
#define CANDIES_H

#include "../main/header.h"

// 
// https://www.hackerrank.com/challenges/candies/problem

// 
// solution 1)
void inc_last(int last, vector<int>& candies, const vector<int>& arr)
{
    candies[last] += 1;
    if (last <= 0) return;

    if (arr[last] < arr[last-1] && candies[last] == candies[last-1]) {
        inc_last(last-1, candies, arr);
    }
}
long candies_1(int n, const vector<int>& arr)
{
    vector<int> candies;
    candies.resize(arr.size(), 1);
    for (int i = 1; i < n; ++i) {
        if (arr[i-1] < arr[i])
          candies[i] = candies[i - 1] + 1;
        else if (arr[i-1] > arr[i])
          inc_last(i, candies, arr);
    }
    long sum = 0;
    for (auto candy : candies) sum += candy;
    return sum;
}

// 
// solution 2), same idea as 1) but withoout recursive
long candies_2(int n, const vector<int>& arr)
{
	vector<int> candies;
    candies.resize(arr.size(), 1);
	for (int i = 1; i < n; i++) {
		if (arr[i] > arr[i - 1]) {
		  candies[i] += candies[i - 1];
		} else {
		  for (int j = i;
			   j > 0 && arr[j] < arr[j - 1] && candies[j] == candies[j - 1];
			   candies[--j] += 1)
			{ }
		}
	}
	long sum = 0;
	for (auto candy : candies) sum += candy;
	return sum;
}

//
// solution 3), ultimate version, best performance!
long candies_3(int n, const vector<int>& arr)
{
    vector<int> dp;
    dp.resize(arr.size(), 1);
    // forward track, guarantee right side higher score gets more candies
    // but doesn't guarantee right side lower score gets less candies
    for(int i = 1; i < n; i++) {
        if(arr[i] > arr[i-1]) {
            dp[i] = dp[i] + dp[i-1]; 
        }
    }
    // back track, guarantee right side lower score gets less candies
    // meaning the left side higher score gets more candies
    for(int i = n-2; i >= 0; i--){
        if(arr[i] > arr[i+1] && dp[i] <= dp[i+1]) {
            dp[i] = dp[i+1] + 1;
        }
    }
    long sum = 0;
    for (auto candy : dp) sum += candy;
    return sum;
}

#endif

