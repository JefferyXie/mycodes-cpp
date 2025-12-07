#pragma once

#include "../core/header.h"

// https://www.hackerrank.com/challenges/gem-stones
int gemstones()
{
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int N = 0;
    std::cin >> N;
    int  count = 0;
    char arr[2][26];
    int  tempN = N;
    while (tempN--) {
        std::memset(*arr, 0, sizeof(arr));

        char elements[100] = {0};
        std::cin >> elements;
        for (int i = 0; i < (int)strlen(elements); ++i) {
            int index = elements[i] - 'a';
            if (arr[0][index] == 0) {
                arr[1][index]++;
                arr[0][index] = 1;
                if (tempN == 0 && arr[1][index] == N) {
                    count++;
                }
            }
        }
    }
    std::cout << count << std::endl;
    return 0;
}

