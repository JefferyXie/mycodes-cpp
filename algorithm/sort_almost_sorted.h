#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/almost-sorted
//

// TODO: buggy, check code here -
// https://github.com/havelessbemore/hackerrank/blob/master/algorithms/sorting/almost-sorted.java

void sort_almost_sorted(std::vector<int> arr)
{
    auto   len             = arr.size();
    size_t ret_lt          = 0;
    size_t ret_rt          = 0;
    size_t lt              = 0;
    size_t rt              = 0;
    bool   start_unordered = false;
    for (size_t i = 1; i < len; ++i) {
        if (start_unordered) {
            if (arr[i] > arr[rt]) {
                if (arr[i] > arr[lt]) {
                    // swap/reverse [lt:rt]
                    if (rt - lt <= 2) {
                        // swap
                        auto t  = arr[lt];
                        arr[lt] = arr[rt];
                        arr[rt] = t;
                    } else {
                        // reverse
                    }
                    ret_lt = lt;
                    ret_rt = rt;
                    // reset and keep going, following numbers must be ordered
                    lt              = 0;
                    rt              = 0;
                    start_unordered = false;
                } else {
                    // TODO: buggy! there have 'swap' case
                    /*
                        arr.assign({
                            92941,
                            894947, // swap 1
                            125082, 864994, 868755,
                            116375, // swap 2
                            911042, 912634
                        });
                    */ std::cout << "no" << std::endl;
                    return;
                }
            } else {
                rt = i;
            }
        } else {
            if (arr[i] > arr[rt]) {
                ++rt;
                continue;
            } else {
                // start having unordered numbers
                // "no" if there's had an operation before,
                // or current arr[i] is less than prior one arr[rt-1]
                if (ret_rt || (rt > 0 && arr[i] < arr[rt - 1])) {
                    std::cout << "no" << std::endl;
                    return;
                } else {
                    lt              = i - 1;
                    rt              = i;
                    start_unordered = true;
                }
            }
        }
    }
    if (!ret_rt) {
        ret_lt = lt;
        ret_rt = rt;
    }
    std::cout << "yes" << std::endl;
    if (ret_rt - ret_lt <= 2) {
        std::cout << "swap ";
    } else {
        std::cout << "reverse ";
    }
    std::cout << ret_lt + 1 << " " << ret_rt + 1 << std::endl;
}

