#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/sam-and-substrings
//
// 972698438521 -> 445677619
//

int substring_sum(std::string n)
{
    using ULL       = unsigned long long;
    const auto base = (long)std::pow(10, 9) + 7;
    const auto len  = n.size();
    ULL        sum  = n[len - 1] - '0';
    ULL        temp = n[len - 1] - '0';
    auto       i    = len - 1;
    while (i) {
        std::string s;
        s.resize(len - i + 1, n[i - 1]);
        temp += std::stoull(s.c_str());
        sum += temp;
        --i;
    }
    return sum % base;
}

