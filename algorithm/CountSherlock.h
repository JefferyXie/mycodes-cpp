#ifndef COUNTSHERLOCK_H
#define COUNTSHERLOCK_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/sherlock-and-gcd
// this is upgraded version of above question -
// 1), for original question, we just need to see if the whole array has gcd > 1
// 2), this version tells how many sub-array meet the criteria
// 3), this version output all the satisfied sub-array

// find the greatest common divisor of a and b
int gcd(int a, int b) {
    return a == 0 ? b : gcd(b%a, a);
}
// find gcd for an array
int gcd(int arr[], int size, int pos) {
    if (size - pos < 2) return -1;
    else if (size - pos == 2) return gcd(arr[pos], arr[pos+1]);
    return gcd(arr, size, pos+1);
}
void print(int arr[], int size) {
    for (int i = 0; i < size; ++i) {
        cout << (i==0 ? "[" : "") << arr[i] << ((i==size-1) ? "]\n" : ",");
    }
}

int CountSherlock(int arr_original[], int size_original, int pos_original, int target[], int size) {
    if (size_original < size || pos_original > size_original) return 0;

    int count = 0;
    if (gcd(target, size, 0) == 1) {
        print(target, size);
        ++count;
    }

    for (int k = pos_original; k < size_original; ++k) {
        target[size] = arr_original[k];
        count += CountSherlock(arr_original, size_original, k+1, target, size+1);
    }
    return count;
}

int runCountSherlock() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int T = 0;
    cin >> T;
    if (T < 1 || T > 10) return -1;
    for (int i = 0; i < T; ++i) {
        int N = 0;
        cin >> N;
        int arr[N];
        for (int j = 0; j < N; ++j) {
            cin >> arr[j];
        }
        int target[N];
        int count = CountSherlock(arr, N, 0, target, 0);
        cout << "Count = " << count << endl;
    }
    return 0;
}

#endif

