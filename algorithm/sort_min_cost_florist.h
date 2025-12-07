#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/greedy-florist/
//

int sort_min_cost_florist(int k, std::vector<int> c)
{
    std::sort(c.begin(), c.end());
    auto len        = c.size();
    int  num_rounds = len / k;
    int  num_last_k = len % k;

    int cost = 0;
    // pick the flowers from most expensive to cheapest, k flowers for each round
    for (int i = 0; i < num_rounds; ++i) {
        for (int j = 0; j < k; ++j) {
            cost += (i + 1) * c[len - (j + i * k) - 1];
        }
    }
    // pick the flowers from cheapest to expensive for leftover
    for (int i = 0; i < num_last_k; ++i) {
        cost += c[i] * (num_rounds + 1);
    }
    return cost;
}

