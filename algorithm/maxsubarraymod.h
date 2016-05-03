#ifndef MAXSUBARRAYMOD_H
#define MAXSUBARRAYMOD_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/maximise-sum
// http://stackoverflow.com/questions/31113993/maximum-subarray-sum-modulo-m
long long MaxSubarrayMod(long long* arr, int length, long long M) {
    long long mods[length];
    memset(mods, 0, sizeof(mods));
    long long sum = 0;
    for (int i = 0; i < length; ++i) {
        sum += arr[i];
        mods[i] = sum%M;
    }
    long long maxMod = 0;
    set<long long> visitedMods;
    for (int i = 0; i < length; ++i) {
        long long mod = mods[i];
        auto it = visitedMods.upper_bound(mod);
        maxMod = max(maxMod, it == visitedMods.end() ? mod : ((mod-*it+M)%M));
        visitedMods.insert(mod);
    }
    return maxMod;
}


#endif


