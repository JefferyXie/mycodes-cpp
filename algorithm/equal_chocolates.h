#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/equal/problem

// common method that calc the least steps needed to make two numbers equal
// by choosing a number from candidates per step
int get_min_steps(int diff)
{
    return diff / 5 + diff % 5 / 2 + diff % 5 % 2 / 1;
}

//
// solution 1), not yet fully read
int equal_chocolates_1(std::vector<int> a)
{
    sort(a.begin(), a.end());

    int delta[] = {0, 1, 2, 5};
    int sum     = std::numeric_limits<int>::max();
    for (unsigned int j = 0; j < sizeof(delta) / sizeof(delta[0]); j++) {
        int ans = 0;
        int d   = 0;
        a[0] -= delta[j];
        for (unsigned int i = 1; i < a.size(); i++) {
            int diff = a[i] - a[i - 1];    // refer to prev elements
            ans += get_min_steps(diff + d);
            d += diff;    // memorizing diffs
        }
        a[0] += delta[j];
        sum = std::min(sum, ans + (delta[j] == 0 ? 0 : 1));
    }
    return sum;
}

//
// solution 2), really smart idea!
/*
https://www.hackerrank.com/challenges/equal/forum/comments/277168

First we must realize that adding to all but the chosen one is the same as
subtracting from only the chosen one.
Then the other idea is that we must find the optimal solution.
For example, 0 4 4 could be solved like this:
0 4 4 --> 0 4 2 --> 0 4 0 --> 0 2 0 --> 0 0 0 -->

But the optimal solution is this:
0 4 4 --> 0 4 -1 --> 0 -1 -1 --> -1 -1 -1 -->

This is what I call the 'base'. and I add it to the delta, and it turns out
you have to make at least 3 trials.
*/
int equal_chocolates_2(std::vector<int> cookies)
{
    sort(cookies.begin(), cookies.end());

    int sum = std::numeric_limits<int>::max();
    for (int base = 0; base < 3; base++) {
        int current_sum = 0;
        for (unsigned int i = 0; i < cookies.size(); i++) {
            int delta = cookies[i] - cookies[0] + base;
            current_sum += get_min_steps(delta);
        }
        sum = std::min(current_sum, sum);
    }
    return sum;
}

