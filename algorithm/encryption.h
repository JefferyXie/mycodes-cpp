#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "../header.h"

// https://www.hackerrank.com/challenges/encryption
string Encryption(string text) {
    int L = text.length();
    int L_floor = floor(sqrt(L));
    int L_ceil = ceil(sqrt(L));
    int row, col;
    if (L_floor*L_ceil == L) {
        row = L_ceil;
        col = L_ceil;
    } else if (L_ceil*L_floor > L) {
        row = L_floor;
        col = L_ceil;
    } else {
        row = L_ceil;
        col = L_ceil;
    }
    string result;
    for (int i = 0; i < col; ++i) {
        for (int j = 0; j < row; ++j) {
            int index = j*col + i;
            if (index < L)
                result += text[index];
        }
        result += ' ';
    }
    return result.erase(result.length()-1,1);
}


#endif


