#ifndef EXTRA_LONG_FACTOR_H
#define EXTRA_LONG_FACTOR_H

#include "../main/header.h"

// 
// https://www.hackerrank.com/challenges/extra-long-factorials
// 

void
extra_long_factorials(int n)
{
    //Initial product = 1
    vector <int> arr{1};

    for(int i = 1; i <= n; i++) {
        int carry = 0;
        //Current size of the number stored in arr
        auto k = arr.size();
        for(size_t j = 0; j < k; j++) {
            arr[j] = arr[j] * i + carry;
            carry = arr[j] / 10;
            arr[j] = arr[j] % 10;
        }
        //Propogate the remaining carry to higher order digits
        while(carry) {
            arr.push_back(carry % 10);
            carry /= 10;
        }
    }
    for(size_t i = arr.size(); i > 0; i--) {
        cout << arr[i-1];
    }
}

#endif

