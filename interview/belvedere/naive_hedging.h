#include <iostream>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
//
// [Belvedere] Naive Hedging Algorithm
//
// In order to properly offload risk, we need to implement a trading algorithm which places trades to offset the risk
// of incoming trades (this is called hedging). Each incoming trade has some risk associated with it, defined as
// quantity * riskPerUnit
// * For example, if an incoming trade has bought 20 units of risk, we should sell 20 units of risk to offset it. If
//   the incoming trade buys 20.9 units of risk, we should still sell 20 units of risk to offset it because we cannot
//   buy fractions of a stock. However, we should remember that there is still 0.9 risk to be covered, and add it to
//   the amount of risk for the next trade.
//
// Given two incoming trades, each with 0.5 units of risk, we should not make any trades when we receive the first
// trade, and then sell 1 when we receive the second trade.
// * For each incoming trade, you'll output the corresponding trade to offset the risk, and the average fill price.
//
// We can define the average fill price of a trade as:
// AvgFillPrice = Sum(quantity_i * price_i) / Sum(quantity_i)
//
// * When we make a trade, we trade against the given market data, affecting the state of the market for future trades.
//   For example, if the best price/quantity in the market to buy was quantity 100 for price $1850, and we bought 75
//   for $1850, there would only be 25 remaining at that price level. If, for example, our next trade was for a quantity
//   of 30, we would execute 25 at the remaining price level, and then the remaining 5 at the next-best price level.
//
// Input:
// * The first two lines of each test case will represent the market data. Each line of market data will always have
//   exactly 3 quantities and exactly 3 prices. You can assume there is enough quantity to fully offset all the risk of
//    all incoming trades.
// * The first line will represent the "offers", or prices you can buy at. The second line will represent the "bids",
//   or prices you can sell at. When trading with the market, we buy at the cheapest price available, and sell at the
//   highest price available.
//
// Market data will be formatted as follows:
// 100 1850.00 200 1850.25 300 1850.50
// 100 1849.75 200 1849.50 300 1849.25
//
// This means that the first 100 purchased will be bought at a price of 1850.00, and the next 200 will be bought at a
// price of 1850.25. Similarly, the first 100 sold will be sold at 1849.75, and the next 200 will be sold at 1849.50
//
// If we were to purchase 300, we would fill 100 at 1850.00, and 200 at 1850.25, for an average fill price of 1850.17
// (rounded to two decimal places).
//
// Incoming trades are formatted as follows:
// +10 0.20
// +15 -0.20
// -40 0.50
//
// This means we traded, in order
// * Buy quantity 10, risk per unit of 0.20 (risk is +2)
// * Buy quantity 15, risk per unit of -0.20 (risk is -3)
// * Sell quantity 40, risk per unit of 0.50 (risk is -20)
//
// NOTE: You have been given skeleton code to help parse this input. You may alter this code as you wish, including not
// using it at all.
//
// Output:
// * We should output the trades required to offset the risk, as well as the average fill price. Buying is represented
//   as a positive quantity, and selling is represented as a negative quantity.
// * The average fill price should output exactly two decimal places, rounded. For example, 1849.6666666 should be
//   rounded to 1849.67.
//
// For the above input, this is the correct output
// -2 1849.75
// 3 1850.00
// 20 1850.00
//
struct naive_order_t {
    naive_order_t(int qty, double prc) : quantity(qty), price(prc * price_divisor) {}
    int                  quantity      = 0;
    int                  price         = 0;
    constexpr static int price_divisor = 100;
};

std::vector<naive_order_t> naive_hedging(
    std::vector<naive_order_t>& book_ask, std::vector<naive_order_t>& book_bid,
    const std::vector<naive_order_t>& trades)
{
    std::vector<naive_order_t> result;
    constexpr int              price_divisor = naive_order_t::price_divisor;
    int                        carry_risk    = 0;
    for (auto& trade : trades) {
        int risk   = trade.price * trade.quantity + carry_risk;
        carry_risk = risk % price_divisor;

        auto hedge = [](int unsigned_qty, auto& side_book) {
            int cum_value = 0;
            int hedge_qty = unsigned_qty;
            for (auto& order : side_book) {
                if (unsigned_qty > order.quantity) {
                    cum_value += order.quantity * order.price;
                    unsigned_qty -= order.quantity;
                    // hedge impacts market orders
                    order.quantity = 0;
                } else {
                    cum_value += unsigned_qty * order.price;
                    // hedge impacts market orders
                    order.quantity -= unsigned_qty;
                    unsigned_qty = 0;
                    break;
                }
            }
            // assume market is big enough to hedge all??
            return (double)cum_value / (hedge_qty * price_divisor);
        };

        int    risk_qty    = risk / price_divisor;
        double hedge_price = 0;
        if (risk_qty < 0) {
            hedge_price = hedge(-1 * risk_qty, book_ask);
        } else if (risk_qty > 0) {
            hedge_price = hedge(risk_qty, book_bid);
        }
        result.emplace_back(naive_order_t{-1 * risk_qty, hedge_price});
    }
    return result;
}

void run_naive_order()
{
    {
        std::cout << "\n--------------------" << std::endl;
        std::vector<naive_order_t> mkt_asks = {
            naive_order_t{100, 1850.00},
            naive_order_t{200, 1850.25},
            naive_order_t{300, 1850.50},
        };
        std::vector<naive_order_t> mkt_bids = {
            naive_order_t{100, 1849.75},
            naive_order_t{200, 1849.50},
            naive_order_t{300, 1849.25},
        };
        std::vector<naive_order_t> trades = {
            naive_order_t{10, 0.20},
            naive_order_t{15, -0.20},
            naive_order_t{-40, 0.50},
        };
        auto orders = naive_hedging(mkt_asks, mkt_bids, trades);
        for (auto& order : orders) {
            ::printf("%d %.02f\n", order.quantity, (double)order.price / order.price_divisor);
        }
        //-2 1849.75
        // 3 1850.00
        // 20 1850.00
    }

    {
        std::cout << "\n--------------------" << std::endl;
        // clang-format off
        std::vector<naive_order_t> mkt_asks = {
            naive_order_t{100, 1850.00},
            naive_order_t{200, 1850.25},
            naive_order_t{300, 1850.50},
        };
        std::vector<naive_order_t> mkt_bids = {
            naive_order_t{100, 1849.75},
            naive_order_t{200, 1849.50},
            naive_order_t{300, 1849.25},
        };
        std::vector<naive_order_t> trades = {
            naive_order_t{15, 0.25},
            naive_order_t{1, 0.25},
            naive_order_t{-15, 0.50},
            naive_order_t{-1, 0.50},
            naive_order_t{-11, 0.25},
            naive_order_t{14, 0.25},
            naive_order_t{1, 0.25},
        };
        // clang-format on
        auto orders = naive_hedging(mkt_asks, mkt_bids, trades);
        for (auto& order : orders) {
            ::printf("%d %.02f\n", order.quantity, (double)order.price / order.price_divisor);
        }
        //-3 1849.75
        //-1 1849.75
        // 7 1850.00
        // 1 1850.00
        // 2 1850.00
        //-2 1849.75
        //-1 1849.75
    }

    {
        std::cout << "\n--------------------" << std::endl;
        // clang-format off
        std::vector<naive_order_t> mkt_asks = {
            naive_order_t{100, 1850.00},
            naive_order_t{200, 1850.25},
            naive_order_t{300, 1850.50},
        };
        std::vector<naive_order_t> mkt_bids = {
            naive_order_t{100, 1849.75},
            naive_order_t{200, 1849.50},
            naive_order_t{300, 1849.25},
        };
        std::vector<naive_order_t> trades = {
            naive_order_t{200, 0.50},
            naive_order_t{200, 0.50},
            naive_order_t{400, 0.50},
            naive_order_t{-400, 0.50},
            naive_order_t{-200, 0.50},
            naive_order_t{-100, 0.50},
        };
        // clang-format on
        auto orders = naive_hedging(mkt_asks, mkt_bids, trades);
        for (auto& order : orders) {
            ::printf("%d %.02f\n", order.quantity, (double)order.price / order.price_divisor);
        }
        //-100 1849.75
        //-100 1849.50
        //-200 1849.37
        // 200 1850.12
        // 100 1850.25
        // 50 1850.50
    }
}

