#ifndef MISSINGNUMBERS_H
#define MISSINGNUMBERS_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/missing-numbers
set<int> FindMissingNumbers(int* A, int n, int* B, int m) {
    set<int> missingSet;
    int min_B = *B;
    int max_B = *B;
    for (int i = 1; i < m; ++i) {
        min_B = min(*(B+i), min_B);
        max_B = max(*(B+i), max_B);
    }
    int existing[max_B-min_B+1];
    memset(existing, 0, sizeof(existing));
    for (int i = 0; i < m; ++i) {
        int index = *(B+i) - min_B;
        existing[index]++;
    }
    for (int i = 0; i < n; ++i) {
        int a = *(A+i);
        if (a > max_B || a < min_B) missingSet.insert(a);
        else --existing[a-min_B];
    }
    for (unsigned int i = 0; i < sizeof(existing)/sizeof(existing[0]); ++i) {
        if (existing[i] != 0) missingSet.insert(i+min_B);
    }
    return missingSet;
}

#endif

