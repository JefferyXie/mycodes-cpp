#ifndef BIGGER_IS_GREATER_H
#define BIGGER_IS_GREATER_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/bigger-is-greater
// 1), starting from end of string, find the character which is smaller than its right side one
// 2), keep recording occurances of checked characters
// 3), once 1) is found, change the character with the one that is LITTLE bigger which is recorded in 2)
// 4), re-order the right side rest string so that it's the smallest by using 2)
void bigger_is_greater() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int t = 0;
    cin >> t;
    const int a_int = (int)'a';
    while (t--) {
        char arr[100];
        char characters[27];
        memset(characters, 0, 27);
        cin >> arr;
        bool bExist = false;
        for (int i = strlen(arr)-1; i >= 0; i--) {
            //char ch = arr[i];
            int ch_index = (int)arr[i]-a_int;
            characters[ch_index]++;
            for (int j = ch_index+1; j < 27; ++j) {
                if (characters[j] <= 0) continue;
                arr[i] = (char)(j+a_int);
                characters[j]--;
                bExist = true;
                break;
            }
            if (bExist) {
                for (int k = 0; k < 27; ++k) {
                    if (characters[k] > 0) {
                        arr[++i] = (char)(k+a_int);
                        characters[k--]--;
                    }
                }
                break;
            }
        }
        cout << (bExist ? arr : "no answer") << endl;
    }
}

#endif

