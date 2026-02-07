#include <iostream>
#include <map>
#include <numeric>
#include <string_view>
#include <unordered_map>

///////////////////////////////////////////////////////////////////////////////
//
// [Belvedere] Concert Ticket Exchange
//
// Concert tickets can be hard to come by, especially when going through the resale market. We would like to build our
// own concert ticket exchange system so that we can view available tickets in real time.
//
// This system will listen to incoming ticket orders for a new concert venue, BT Arena. We'd like to build a system
// that listens to concert ticket orders and provides fans with this market information in the form of a price ladder.
// Concert ticket orders arrive as a stream of events that can either add an order to our market data book or delete an
// order from our book. We also want to be able to delete all orders at a given price level.
//
// Add orders come with the following information:
// - Action - ADD
// - OrderId - unique identifier for order
// - Artist - concert performer, such as "TaylorSwift" or "Drake"
// - Price - Price for the order
// - Quantity - positive quantity is a buy order and negative quantity is a sell order
//
// Delete orders come with the following information:
// - Action - DEL
// - Artist - concert performer, such as "TaylorSwift" or "Drake"
// - OrderId - Unique identifier for the order to be deleted
//
// Delete price levels come with the following information:
// - Action - DEL_PRICE
// - Artist - concert performer, such as "TaylorSwift" or "Drake"
// - Price - price level to be deleted
//
// We'd like to keep track of these events so that fans can request a market view in the form of a price ladder. A
// price ladder is made up of n price levels for buy orders and sell orders. A price level consists of the Price,
// BuyQuantity, and SellQuantity. A price level will either have a non-zero BuyQuantity or SellQuantity, but not both.
// For buy orders, the best price is the highest price because that is the price a ticket holder would sell at.
// Conversely, the best price for sell orders is the lowest price. The streamed events will guarantee at least n price
// levels for both buys and sell.
//
// Input:
// Repeated number of order operations in the following format:
// We can get orders for different artists in the same stream of operations.
// The last line of input will the artist and number of price levels that we would like to view in our ladder for both
// buy and sell orders.
//
// Example Input:
// ADD 1 TaylorSwift 100 10
// ADD 2 TaylorSwift 101 -10
// ADD 3 TaylorSwift 99 5
// ADD 4 TaylorSwift 102 -5
// ADD 5 TaylorSwift 100 2
// ADD 6 Drake 95 2
// DEL 1 TaylorSwift
// TaylorSwift 2
//
// Output:
// TaylorSwift 102 5  101 10 2 100 5 99
//
// The price ladder should begin with the artist's name and should be followed by a repeated list of levels. The levels
// should be sorted by price in descending order and contain NumberOfPriceLevels for buy and sell orders. Levels with 0
// BuyQuantity and 0 SellQuantity should not be printed. 
//
class ticket_system_t final
{
public:
    ticket_system_t() = default;
    void read(std::string_view input)
    {
        std::cout << "\t read=" << input << std::endl;

        size_t pos       = 0;
        auto   get_token = [&]() {
            constexpr char delimiter = ' ';
            const auto     pos_start = pos;
            auto           pos_end   = input.find(delimiter, pos);
            if (pos_end != std::string_view::npos) {
                pos = pos_end + 1;
                return input.substr(pos_start, pos_end - pos_start);
            }
            pos = std::string_view::npos;
            return input.substr(pos_start);
        };

        auto action = get_token();
        if (action == "ADD") {
            int  order_id = std::stoi(std::string{get_token()});
            auto artist   = get_token();
            int  price    = std::stoi(std::string{get_token()});
            int  qty      = std::stoi(std::string{get_token()});
            add(artist, order_id, price, qty);
        } else if (action == "DEL") {
            int  order_id = std::stoi(std::string{get_token()});
            auto artist   = get_token();
            remove(artist, order_id);
        } else if (action == "DEL_PRICE") {
            auto artist = get_token();
            int  price  = std::stoi(std::string{get_token()});
            remove_price(artist, price);
        } else {
            auto artist = std::move(action);
            auto levels = std::stoi(std::string{get_token()});
            print(artist, levels);
        }
    }
    void add(std::string_view artist, int order_id, int price, int qty)
    {
        std::cout << "\t add: artist=" << artist << ", order_id=" << order_id << ", price=" << price << ", qty=" << qty
                  << std::endl;

        auto& book = tickets_[std::string{artist}];
        if (qty < 0) {
            book.asks_[price] += (-1 * qty);
        } else if (qty > 0) {
            book.bids_[price] += qty;
        }
        book.orders_[order_id] = order_t{
            .price    = price,
            .quantity = qty,
        };
    }
    void remove(std::string_view artist, int order_id)
    {
        std::cout << "\t remove: artist=" << artist << ", order_id=" << order_id << std::endl;

        auto& book = tickets_[std::string{artist}];
        if (auto iter = book.orders_.find(order_id); iter != book.orders_.end()) {
            const auto& order = iter->second;
            if (order.quantity < 0) {
                book.asks_[order.price] += order.quantity;
            } else {
                book.bids_[order.price] -= order.quantity;
            }
            book.orders_.erase(order_id);
        }
    }
    void remove_price(std::string_view artist, int price)
    {
        std::cout << "\t remove_price: artist=" << artist << ", price=" << price << std::endl;

        auto&      book    = tickets_[std::string{artist}];
        const auto top_ask = (book.asks_.size() > 0 ? book.asks_.begin()->first : std::numeric_limits<int>::max());
        const auto top_bid = (book.bids_.size() > 0 ? book.bids_.begin()->first : std::numeric_limits<int>::min());
        if (price >= top_ask) {
            book.asks_.erase(price);
        } else if (price <= top_bid) {
            book.bids_.erase(price);
        }
    }
    void print(std::string_view artist, int levels)
    {
        std::cout << artist;

        auto& book = tickets_[std::string{artist}];
        auto  iter = book.asks_.begin();
        std::advance(iter, std::min(levels, (int)book.asks_.size()));
        while (iter-- != book.asks_.begin()) {
            std::cout << " " << iter->first << " " << iter->second;
        }

        for (auto iter = book.bids_.begin(); iter != book.bids_.end() && levels-- > 0; ++iter) {
            std::cout << " " << iter->first << " " << iter->second;
        }
    }

private:
    struct order_t {
        int price    = 0;
        int quantity = 0;
    };
    struct ticket_book_t {
        // price <> unsigned level qty
        std::map<int, int, std::less<>>    asks_;
        std::map<int, int, std::greater<>> bids_;
        // order id <> order
        std::unordered_map<int, order_t> orders_;
    };
    std::unordered_map<std::string, ticket_book_t> tickets_;
};

void run_ticket_system()
{
    // clang-format off
    ticket_system_t system;
    for (auto& input : {std::string_view{"ADD 1 TaylorSwift 100 10"},
                        std::string_view{"ADD 2 TaylorSwift 101 -10"},
                        std::string_view{"ADD 3 TaylorSwift 99 5"},
                        std::string_view{"ADD 4 TaylorSwift 102 -5"},
                        std::string_view{"ADD 5 TaylorSwift 100 2"},
                        std::string_view{"ADD 6 Drake 95 2"},
                        std::string_view{"DEL 1 TaylorSwift"},
                        std::string_view{"TaylorSwift 2"}}) {
        system.read(input);
    }
    // clang-format on
}

