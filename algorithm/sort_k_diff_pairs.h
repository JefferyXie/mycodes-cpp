#ifndef SORT_K_DIFF_PAIRS_H
#define SORT_K_DIFF_PAIRS_H

#include "../main/header.h"

//
// https://www.hackerrank.com/challenges/pairs
//

int sort_k_diff_pairs(std::vector<int>& a, int k)
{
    int ans = 0;
    sort(a.begin(), a.end());
    int i = 0, j = i + 1;
    while (j < (int)a.size()) {
        if (a[j] - a[i] == k) {
            ans++;
            i++;
            j++;
        } else if (a[j] - a[i] > k) {
            i++;
            if (i == j)
                j++;
        } else {
            j++;
        }
    }
    return ans;
}

#endif
