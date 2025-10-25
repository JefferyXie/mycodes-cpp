#ifndef STOCK_MAX_PROFIT_H
#define STOCK_MAX_PROFIT_H

#include "../main/header.h"

//
// https://www.hackerrank.com/challenges/stockmax/problem
//
// you can hold any number of shares in your portfolio, and sell any number of shares on one time
//

//
// buy all dates before the max price date, keep finding max price date afterwards over and over instead of forward
// iteration, run the calc backwards makes the logic clear and easy
//
int stock_max_profit(std::vector<int> prices)
{
    auto len       = prices.size();
    int  profit    = 0;
    int  max_price = 0;
    while (len--) {
        max_price = std::max(prices[len], max_price);
        profit += max_price - prices[len];
    }
    return profit;
}

/*
// another solution with dynamic programming, on each day
// find three numbers: buy stock on the day, sell stock, or do nothing
int stock_max_profit_v2(vector<int> prices)
{
    int profit_has_position     = 0;
    int profit_no_position      = 0;
    for (size_t i = 1; i < prices.size(); ++i) {
        int php                 = profit_has_position;
        int pnp                 = profit_no_position;
        // do nothing (php), or buy (profit no change, so pnp)
        profit_has_position     = std::max({php, pnp});
        // sell, or do nothing (pnp)
        profit_no_position      = std::max({php + prices[i] - prices[i-1], pnp});

        cout << "i=" << i << ": has=" << profit_has_position << ", no=" << profit_no_position << endl;
    }
    return std::max({profit_has_position, profit_no_position});
}
*/

#endif
