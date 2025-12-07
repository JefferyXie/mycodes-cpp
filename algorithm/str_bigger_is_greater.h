#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/bigger-is-greater
// 1), starting from end of string, find the character which is smaller than its right side one
// 2), keep recording occurances of checked characters
// 3), once 1) is found, change the character with the one that is LITTLE bigger which is recorded in 2)
// 4), re-order the right side rest string so that it's the smallest by using 2)
//
void str_bigger_is_greater_v1()
{
    int t = 0;
    std::cin >> t;
    const int a_int = (int)'a';
    while (t--) {
        char arr[100];
        char characters[27];
        memset(characters, 0, 27);
        std::cin >> arr;
        bool bExist = false;
        for (int i = strlen(arr) - 1; i >= 0; i--) {
            // char ch = arr[i];
            int ch_index = (int)arr[i] - a_int;
            characters[ch_index]++;
            for (int j = ch_index + 1; j < 27; ++j) {
                if (characters[j] <= 0)
                    continue;
                arr[i] = (char)(j + a_int);
                characters[j]--;
                bExist = true;
                break;
            }
            if (bExist) {
                for (int k = 0; k < 27; ++k) {
                    if (characters[k] > 0) {
                        arr[++i] = (char)(k + a_int);
                        characters[k--]--;
                    }
                }
                break;
            }
        }
        std::cout << (bExist ? arr : "no answer") << std::endl;
    }
}

std::string str_bigger_is_greater_v2(std::string w)
{
    auto len = w.size();
    if (len <= 1)
        return "no answer";

    auto i = len - 1;
    do {
        if (w[i] > w[i - 1])
            break;
    } while (--i);
    if (i == 0)
        return "no answer";

    // find w[i-1]'s upper bound from [i, len)
    auto bound     = std::upper_bound(w.rbegin(), w.rbegin() + len - i, w[i - 1]);
    auto bound_idx = len - std::distance(w.rbegin(), bound) - 1;
    //    cout << "pos:" << i << ",bound_idx:" << bound_idx << endl;

    // swap w[bound_idx] and w[i-1]
    auto t       = w[i - 1];
    w[i - 1]     = w[bound_idx];
    w[bound_idx] = t;

    // reverse [i, len)
    auto lt = i;
    auto rt = len - 1;
    while (lt < rt) {
        auto t = w[lt];
        w[lt]  = w[rt];
        w[rt]  = t;
        ++lt;
        --rt;
    }
    return w;
}

