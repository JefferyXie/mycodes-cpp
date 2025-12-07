#pragma once

#include "../core/header.h"

/*
 https://www.hackerrank.com/challenges/deque-stl

 Given a set of arrays of size  and an integer , you have to find the maximum integer for each and every contiguous
 subarray of size  for each of the given arrays.

 Input: 3 4 6 3 4, k=2
 Output: 4 6 6 4

 Input: 3 4 5 8 1 4 10, k=4
 Output: 8 8 8 10
*/
std::string print_k_max_1(int arr[], int n, int k)
{
    if (n < k) {
        return "";
    }
    std::string     result;
    int             maxEle = 0;
    int             popped = 0;
    std::deque<int> dq;
    for (int i = 0; i < n; ++i) {
        int pushed = arr[i];
        dq.push_back(pushed);
        if (popped < maxEle) {
            maxEle = std::max(maxEle, pushed);
        } else if (popped <= pushed) {
            maxEle = pushed;
        } else {
            maxEle = 0;
            for_each(begin(dq), end(dq), [&maxEle](int a) {
                maxEle = std::max(maxEle, a);
            });
        }
        if ((int)dq.size() == k) {
            result += std::to_string(maxEle) + ' ';
            // cout << maxEle << ' ';
            popped = dq.front();
            dq.pop_front();
        }
    }
    return result.erase(result.size() - 1, 1);
}

// version 2:
// 1) don't search max value in the deque
// 2) the first element in deque always the biggest
// 3) the last element in deque always the smallest
// 4) keep removing last elements if newly added element is big
std::deque<int> q;
void            add(int x)
{
    while (!q.empty() && q.back() < x)
        q.pop_back();
    q.push_back(x);
}
void remove(int x)
{
    if (!q.empty() && q.front() == x) {
        q.pop_front();
    }
}
std::string print_k_max_2(int arr[], int n, int k)
{
    if (n < k)
        return "";
    q.clear();
    std::string result;
    for (int i = 0; i < k; ++i) {
        add(arr[i]);
    }
    for (int i = k; i <= n; ++i) {
        result += std::to_string(q.front()) + ' ';
        // cout << q.front() << ' ';
        remove(arr[i - k]);
        add(arr[i]);
    }
    return result.erase(result.size() - 1, 1);
}

