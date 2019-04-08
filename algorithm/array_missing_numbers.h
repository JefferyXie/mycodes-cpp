#ifndef ARRAY_MISSING_NUMBERS_H
#define ARRAY_MISSING_NUMBERS_H

#include "../main/header.h"

// 
// https://www.hackerrank.com/challenges/missing-numbers
// 

vector<int>
array_missing_numbers(vector<int> arr, vector<int> brr)
{
    vector<int> freq(200);
    int a0 = arr[0];
    for (size_t i = 0; i < arr.size(); ++i) {
        ++freq[arr[i]-a0+100];
    }
    for (size_t i = 0; i < brr.size(); ++i) {
        --freq[brr[i]-a0+100];
    }
    vector<int> ret;
    for (size_t i = 0; i < freq.size(); ++i) {
        if (freq[i]) {
            ret.push_back(i+a0-100);
        }
    }
    return ret;
}

#endif
