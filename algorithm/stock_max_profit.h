#ifndef STOCK_MAX_PROFIT_H
#define STOCK_MAX_PROFIT_H

#include "../main/header.h"

// 
// https://www.hackerrank.com/challenges/stockmax/problem
// 

//
// buy all dates before the max price date, keep finding max price date afterwards over and over
// instead of forward iteration, run the calc backwards makes the logic clear and easy
// 
int stock_max_profit(vector<int> prices)
{
	auto len = prices.size();
	int profit = 0;
	int max_price = 0;
	while (len--) {
		max_price = max(prices[len], max_price);
		profit += max_price - prices[len];
	}
	return profit;
}

#endif
