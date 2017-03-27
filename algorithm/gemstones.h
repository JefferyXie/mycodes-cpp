#ifndef GEMSTONES_H
#define GEMSTONES_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/gem-stones
int gemstones() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int N = 0;
    cin >> N;
    int count = 0;
    char arr[2][26];
    int tempN = N;
    while (tempN--) {
        memset(*arr, 0, sizeof(arr));

        char elements[100] = {0};
        cin >> elements;
        for (int i = 0; i < (int)strlen(elements); ++i) {
            int index = elements[i]-'a';
            if (arr[0][index] == 0) {
                arr[1][index]++;
                arr[0][index] = 1;
                if (tempN == 0 && arr[1][index] == N) {
                    count++;
                }
            }
        }
    }
    cout << count << endl;
    return 0;
}

#endif
