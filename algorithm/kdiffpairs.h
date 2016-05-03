#ifndef KDIFFPAIRS_H
#define KDIFFPAIRS_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/pairs
int KDiffPairs(vector<int>& a, int k) {
    int ans=0;
    sort(a.begin(),a.end());
    int i=0,j=i+1;
    while (j < a.size()) {
        if (a[j]-a[i] == k) {
            ans++;
            i++;
            j++;
        } else if (a[j]-a[i] > k) {
            i++;
            if(i==j) j++;
        } else {
            j++;
        }
    }
    return ans;
}

#endif


