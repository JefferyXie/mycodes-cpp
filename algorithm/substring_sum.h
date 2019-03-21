#ifndef SUBSTRING_SUM_H
#define SUBSTRING_SUM_H

#include "../main/header.h"

// 
// https://www.hackerrank.com/challenges/sam-and-substrings
// 
// 972698438521 -> 445677619
// 

int substring_sum(string n)
{
    using ULL = unsigned long long;
    auto base = (long)pow(10, 9)+7;
    auto len = n.size();
    ULL sum = n[len-1]-'0';
    ULL temp = n[len-1]-'0';
    auto i = len-1;
    while (i) {
        string s;
        s.resize(len - i + 1, n[i - 1]);
        temp += stoull(s.c_str());
        sum += temp;
        --i;
    }
    return sum%base;
}

#endif
