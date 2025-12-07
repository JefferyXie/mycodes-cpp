#pragma once

#include "../core/header.h"

//
// Question from Bloomberg
int max_dollar_stick(int* len_to_prices, int stick_len)
{
    int dollars = 0;
    for (int curr_piece_len = 1; curr_piece_len <= stick_len; ++curr_piece_len) {
        int dollars_temp = *(len_to_prices + curr_piece_len - 1);
        dollars_temp += max_dollar_stick(len_to_prices, stick_len - curr_piece_len);
        if (dollars_temp > dollars) {
            dollars = dollars_temp;
        }
    }
    return dollars;
}

void run_max_dollar_stick()
{
    int prices_1[] = {1, 3};
    std::cout << max_dollar_stick(prices_1, sizeof(prices_1) / sizeof(prices_1[0])) << std::endl;
    int prices_2[] = {1, 3, 4};
    std::cout << max_dollar_stick(prices_2, sizeof(prices_2) / sizeof(prices_2[0])) << std::endl;
    int prices_3[] = {1, 4, 5, 7};
    std::cout << max_dollar_stick(prices_3, sizeof(prices_3) / sizeof(prices_3[0])) << std::endl;
    int prices_4[] = {2, 3, 4, 8, 9};
    std::cout << max_dollar_stick(prices_4, sizeof(prices_4) / sizeof(prices_4[0])) << std::endl;
}

