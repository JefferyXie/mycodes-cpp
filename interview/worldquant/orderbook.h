#ifndef WORLDQUANT_H
#define WORLDQUANT_H

#include <assert.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <array>
#include <unordered_map>
#include <utility>

using price_t         = uint64_t;
using quantity_t      = uint64_t;
using instrument_id_t = uint32_t;
using order_id_t      = uint64_t;

enum matching_result_code_t {
    Error,
    NoFill,
    FullFilled,
    PartialFilled,
};

enum side_t {
    Bid,
    Ask,
};

template <side_t SIDE>
struct side_extractor_t {
};

template <>
struct side_extractor_t<side_t::Bid> {
    static bool crossed(price_t agg_bid_price, price_t resting_ask_price) { return agg_bid_price >= resting_ask_price; }
    template <typename T, typename U>
    static T& get_this_side(T& bid, U& ask)
    {
        (void)ask;
        return bid;
    }
    template <typename T, typename U>
    static U& get_other_side(T& bid, U& ask)
    {
        (void)bid;
        return ask;
    }
};
template <>
struct side_extractor_t<side_t::Ask> {
    static bool crossed(price_t agg_ask_price, price_t resting_bid_price) { return agg_ask_price <= resting_bid_price; }
    template <typename T, typename U>
    static U& get_this_side(T& bid, U& ask)
    {
        (void)bid;
        return ask;
    }
    template <typename T, typename U>
    static T& get_other_side(T& bid, U& ask)
    {
        (void)ask;
        return bid;
    }
};

struct order_t;
struct order_level_t {
    order_t* head_order{nullptr};
    order_t* tail_order{nullptr};
};

struct order_t {
    side_t         side;
    order_id_t     id{0};
    price_t        price{0};
    quantity_t     quantity{0};
    order_t*       next_order{nullptr};
    order_t*       prev_order{nullptr};
    order_level_t* level{nullptr};
};

std::ostream& operator<<(std::ostream& os, const order_t& order)
{
    os << "{order: side=" << (order.side == side_t::Bid ? "B" : "A") << ", id=" << order.id << ", price=" << order.price
       << ", quantity=" << order.quantity << "}";
    return os;
}

template <typename Comp>
using side_order_book_t = std::map<price_t, order_level_t, Comp>;

// TODO: this pool is a bit buggy, :((((
// This implementation is same as core/pool_allocator.h
// They share same problem comparing to __learn/allocator/Allocator.h - each time when allocating new chunk, std::vector
// will do resize/reserve, and a loop that add new elements one by one.
//
// __learn/allocator/Allocator.h does it better: MemoryPool
// 1) MemoryPool::Buffer is linked data block/chunk, new object is allocated from this chunk without changing the chunk;
// 2) MemoryPool::Block behaves as linked node for deallocated/freed objects; it's reinterpret_cast from target object pointer - very smart idea!
//
template <typename T>
class data_pool_t
{
public:
    data_pool_t(uint32_t init_size = 1, uint32_t chunk_size = 1)
    {
        chunk_size_ = chunk_size;
        allocate_more(init_size);
    }
    ~data_pool_t() {
        for (auto chunk : all_chunks_) {
            // TODO: make sure this work even if std::vector<T*>
            delete[] chunk;
        }
    }

    auto allocate_order()
    {
        std::cout << "allocate_order: allocated one order." << std::endl;
        // return new order_t{};
        if (pool_.empty()) {
            allocate_more(chunk_size_);
        }
        assert(!pool_.empty());
        auto obj = pool_.back();
        pool_.pop_back();
        return obj;
    }
    void release_order(order_t*& order)
    {
        std::cout << "release_order: released order=" << *order << std::endl;
        *order = order_t{};
        // delete order;
        // order = nullptr;

        pool_.emplace_back(order);
    }

    void allocate_more(uint32_t size)
    {
        std::cout << "allocate_more: size=" << size << ", pool.size=" << pool_.size() << std::endl;

        pool_.reserve(pool_.size() + size);

        auto objs = new T[size];
        assert(objs);

        for (uint32_t i = 0; i < sizeof(T) / sizeof(T[0]); ++i) {
            pool_.emplace_back(objs + i);
        }

        all_chunks_.push_back(objs);
    }

private:
    uint32_t        chunk_size_{0};
    std::vector<T*> pool_;
    std::vector<T*> all_chunks_;
};

struct order_pool_t {
    auto allocate_order()
    {
        std::cout << "allocate_order: allocated one order." << std::endl;
        return new order_t{};
    }
    void release_order(order_t*& order)
    {
        std::cout << "release_order: released order=" << *order << std::endl;
        *order = order_t{};
        delete order;
        order = nullptr;
    }
};

class order_book_t
{
public:
    auto& side_book_bid() const { return bids_; }
    auto& side_book_ask() const { return asks_; }

    void add_order(side_t side, order_id_t id, price_t price, quantity_t quantity)
    {
        if (id == 0 || price == 0 || quantity == 0) {
            return;
        }

        auto order      = order_pool_.allocate_order();
        order->side     = side;
        order->id       = id;
        order->price    = price;
        order->quantity = quantity;

        if (auto result = orders_.emplace(id, order); !result.second) {
            std::cout << "add_order: duplicate order id, order=" << *order << std::endl;
            return;
        }

        auto do_add_order = [order](auto& side_book) {
            auto iter_level = side_book.find(order->price);
            if (iter_level != side_book.end()) {
                auto tail = iter_level->second.tail_order;
                assert(tail && tail->price == order->price);

                std::cout << "add_order: add to exsiting level, tail=" << *tail << ", new=" << *order << std::endl;

                tail->next_order  = order;
                order->prev_order = tail;
                order->next_order = nullptr;
                order->level      = &iter_level->second;

                iter_level->second.tail_order = order;
            } else {
                auto& level      = side_book[order->price];
                level.head_order = order;
                level.tail_order = order;
                order->level     = &level;

                std::cout << "add_order: create new level, new=" << *order << std::endl;
            }
        };
        if (order->side == side_t::Bid) {
            do_add_order(bids_);
        } else {
            do_add_order(asks_);
        }
    }
    void remove_order(order_id_t id)
    {
        if (auto iter_order = orders_.find(id); iter_order != orders_.end()) {
            auto order = iter_order->second;
            orders_.erase(iter_order);

            if (order->prev_order) {
                order->prev_order->next_order = order->next_order;
            }
            if (order->next_order) {
                order->next_order->prev_order = order->prev_order;
            }

            auto level = order->level;
            assert(level);
            if (level->head_order == order) {
                level->head_order = order->next_order;
            }
            if (level->tail_order == order) {
                level->tail_order = order->prev_order;
            }

            std::cout << "remove_order: removed order=" << *order << std::endl;

            if (!level->head_order) {
                if (order->side == side_t::Bid) {
                    bids_.erase(order->price);
                } else {
                    asks_.erase(order->price);
                }
                std::cout << "remove_order: removed level, order=" << *order << std::endl;
            }

            order_pool_.release_order(order);
        } else {
            std::cout << "remove_order: failed to find order, id=" << id << std::endl;
        }
    }
    void modify_order(order_id_t id, price_t new_price, quantity_t new_quantity)
    {
        if (id == 0 || new_price == 0) {
            std::cout << "modify_order: invalid, id=" << id << ", new_price=" << new_price
                      << ", new_quantity=" << new_quantity << std::endl;
            return;
        }

        auto existing_order = find_order(id);
        if (!existing_order) {
            std::cout << "modify_order: failed to find target order, id=" << id << std::endl;
            return;
        }

        std::cout << "modify_order: id=" << id << ", new_price=" << new_price << ", new_quantity=" << new_quantity
                  << ", existing=" << *existing_order << std::endl;

        auto cancel_order = [this, id]() {
            // optimization
            remove_order(id);
        };

        if (new_quantity == 0) {
            return cancel_order();
        }

        auto change_price = [this, existing_order, id, new_price, new_quantity]() {
            // optimization
            remove_order(id);
            add_order(existing_order->side, id, new_price, new_quantity);
        };
        if (new_price != existing_order->price) {
            return change_price();
        }

        auto increase_qty = [this, existing_order, id, new_price, new_quantity]() {
            // optimization
            remove_order(id);
            add_order(existing_order->side, id, new_price, new_quantity);
        };
        if (new_quantity > existing_order->quantity) {
            return increase_qty();
        }

        auto decrease_qty = [new_quantity, existing_order]() {
            existing_order->quantity = new_quantity;
        };
        if (new_quantity < existing_order->quantity) {
            return decrease_qty();
        }

        std::cout << "modify_order: unknown modification, id=" << id << ", new_price=" << new_price
                  << ", new_quantity=" << new_quantity << ", existing=" << *existing_order << std::endl;
    }
    order_t* find_order(order_id_t id)
    {
        if (auto iter_order = orders_.find(id); iter_order != orders_.end()) {
            return iter_order->second;
        }
        return nullptr;
    }

    auto to_string()
    {
        std::ostringstream oss;
        oss << "Ask:\n";
        for (auto iter = asks_.rbegin(); iter != asks_.rend(); ++iter) {
            if (iter != asks_.rbegin()) {
                oss << "\t\t"
                    << "--- --- --- ---"
                    << "\n";
            }

            auto order = iter->second.tail_order;
            while (order) {
                oss << "\t\t" << order->id << "\t" << order->quantity << " @ " << order->price << "\n";
                order = order->prev_order;
            }
        }

        oss << "\t"
            << "--- --- --- --- --- --- --- ---"
            << "\n";

        oss << "Bid:\n";
        for (auto iter = bids_.begin(); iter != bids_.end(); ++iter) {
            if (iter != bids_.begin()) {
                oss << "\t\t"
                    << "--- --- --- ---"
                    << "\n";
            }

            auto order = iter->second.head_order;
            while (order) {
                oss << "\t\t" << order->id << "\t" << order->quantity << " @ " << order->price << "\n";
                order = order->next_order;
            }
        }

        oss << "--- --- --- --- --- --- --- ---"
            << "\n";
        oss << "All orders:\n";
        for (auto& [id, order] : orders_) {
            oss << "\t" << (order->side == side_t::Bid ? "Bid" : "Ask") << "\t" << order->id << "\t" << order->quantity
                << " @ " << order->price << "\n";
        }
        return oss.str();
    }

private:
    // data_pool_t<order_t>                     order_pool_;
    order_pool_t                             order_pool_;
    side_order_book_t<std::greater<>>        bids_;
    side_order_book_t<std::less<>>           asks_;
    std::unordered_map<order_id_t, order_t*> orders_;
};

struct matching_result_t {
    matching_result_code_t result_code{matching_result_code_t::Error};
    quantity_t             filled_quantity{0};
    std::vector<order_t>   matching_orders;
};

class instrument_order_book_map_t
{
public:
    explicit instrument_order_book_map_t(const std::initializer_list<instrument_id_t>& ids)
    {
        for (auto id : ids) {
            std::ignore = books_[id];
        }
    }
    order_book_t* find_order_book(instrument_id_t id)
    {
        if (auto iter = books_.find(id); iter != books_.end()) {
            return &iter->second;
        }
        return nullptr;
    }

private:
    std::unordered_map<instrument_id_t, order_book_t> books_;
};

template <typename MatchingAlgoT, typename InstrumentOrderBookMapT>
class matching_engine_t
{
public:
    explicit matching_engine_t(const std::initializer_list<instrument_id_t>& instruments)
        : instrument_order_books_{instruments}
    {
    }

    template <side_t SIDE>
    matching_result_t
    handle_order_new(instrument_id_t instrument, order_id_t order_id, price_t price, quantity_t quantity)
    {
        std::cout << "handle_order_new<" << (SIDE == side_t::Ask ? "A" : "B") << ">: instrument=" << instrument
                  << ", order_id=" << order_id << ", price=" << price << ", quantity=" << quantity << std::endl;

        auto order_book = instrument_order_books_.find_order_book(instrument);
        if (!order_book) {
            return matching_result_t{};
        }

        matching_result_t result;
        algo_.template try_matching<SIDE>(result, *order_book, price, quantity);

        assert(quantity >= result.filled_quantity);

        if (result.filled_quantity == quantity) {
            result.result_code = matching_result_code_t::FullFilled;
        } else if (result.filled_quantity < quantity) {
            if (result.filled_quantity == 0) {
                result.result_code = matching_result_code_t::NoFill;
            } else {
                result.result_code = matching_result_code_t::PartialFilled;
            }

            quantity -= result.filled_quantity;
            order_book->add_order(SIDE, order_id, price, quantity);
        }

        std::cout << order_book->to_string() << std::endl;
        return result;
    }
    matching_result_t handle_order_cancel(instrument_id_t instrument, order_id_t order_id)
    {
        std::cout << "handle_order_cancel: instrument=" << instrument << ", order_id=" << order_id << std::endl;

        auto order_book = instrument_order_books_.find_order_book(instrument);
        if (!order_book) {
            return matching_result_t{};
        }
        order_book->remove_order(order_id);

        std::cout << order_book->to_string() << std::endl;
        return matching_result_t{
            .result_code = matching_result_code_t::NoFill,
        };
    }
    template <side_t SIDE>
    matching_result_t handle_order_replacement(
        instrument_id_t instrument, order_id_t order_id, price_t new_price, quantity_t new_quantity)
    {
        std::cout << "handle_order_replacement<" << (SIDE == side_t::Ask ? "A" : "B") << ">: instrument=" << instrument
                  << ", order_id=" << order_id << ", new_price=" << new_price << ", new_quantity=" << new_quantity
                  << std::endl;

        auto order_book = instrument_order_books_.find_order_book(instrument);
        if (!order_book) {
            return matching_result_t{};
        }

        auto existing_order = order_book->find_order(order_id);
        if (!existing_order) {
            return matching_result_t{};
        }

        // assert(SIDE == existing_order->side);

        matching_result_t result;
        algo_.template try_matching<SIDE>(result, *order_book, new_price, new_quantity);

        assert(new_quantity >= result.filled_quantity);

        if (result.filled_quantity == new_quantity) {
            result.result_code = matching_result_code_t::FullFilled;
        } else if (result.filled_quantity < new_quantity) {
            if (result.filled_quantity == 0) {
                result.result_code = matching_result_code_t::NoFill;
            } else {
                result.result_code = matching_result_code_t::PartialFilled;
            }

            new_quantity -= result.filled_quantity;
            order_book->modify_order(order_id, new_price, new_quantity);
        }

        std::cout << order_book->to_string() << std::endl;
        return result;
    }

    order_t* find_order(instrument_id_t instrument, order_id_t order_id)
    {
        if (auto order_book = instrument_order_books_.find_order_book(instrument); order_book) {
            return order_book->find_order(order_id);
        }
        return nullptr;
    }

private:
    MatchingAlgoT           algo_;
    InstrumentOrderBookMapT instrument_order_books_;
};

class matching_algo_FIFO_t
{
public:
    template <side_t SIDE>
    void try_matching(matching_result_t& result, order_book_t& resting_book, price_t agg_price, quantity_t agg_quantity)
    {
        auto  agg_residual_qty = agg_quantity;
        auto& other_side_book =
            side_extractor_t<SIDE>::get_other_side(resting_book.side_book_bid(), resting_book.side_book_ask());

        while (true) {
            auto iter_other_side_level = other_side_book.begin();
            if (iter_other_side_level == other_side_book.end()) {
                break;
            }

            auto order = iter_other_side_level->second.head_order;
            assert(order);

            if (!side_extractor_t<SIDE>::crossed(agg_price, order->price)) {
                break;
            }

            while (order) {
                if (order->quantity >= agg_residual_qty) {
                    resting_book.modify_order(order->id, order->price, order->quantity - agg_residual_qty);

                    result.matching_orders.push_back(order_t{
                        .side     = order->side,
                        .id       = order->id,
                        .price    = order->price,
                        .quantity = agg_residual_qty,
                    });
                    result.filled_quantity += agg_residual_qty;

                    agg_residual_qty = 0;

                    return;
                }

                agg_residual_qty -= order->quantity;
                result.matching_orders.push_back(order_t{
                    .side     = order->side,
                    .id       = order->id,
                    .price    = order->price,
                    .quantity = order->quantity,
                });
                result.filled_quantity += order->quantity;

                auto empty_order = order;
                order            = order->next_order;

                // remove order after updating 'order'
                resting_book.remove_order(empty_order->id);
            }
        }
    }
};

class matching_algo_ProRata_t
{
public:
    template <side_t SIDE>
    void try_matching(matching_result_t& result, order_book_t& resting_book, price_t agg_price, quantity_t agg_quantity)
    {
        auto  agg_residual_qty = agg_quantity;
        auto& other_side_book =
            side_extractor_t<SIDE>::get_other_side(resting_book.side_book_bid(), resting_book.side_book_ask());

        while (true) {
            auto iter_other_side_level = other_side_book.begin();
            if (iter_other_side_level == other_side_book.end()) {
                break;
            }

            auto order = iter_other_side_level->second.head_order;
            assert(order);

            if (!side_extractor_t<SIDE>::crossed(agg_price, order->price)) {
                break;
            }

            quantity_t level_total_qty = 0;
            {
                // optimization
                auto tmp_order = order;
                while (tmp_order) {
                    level_total_qty += tmp_order->quantity;
                    tmp_order = tmp_order->next_order;
                }
            }

            auto calc_allocation = [](quantity_t agg_residual_qty, quantity_t level_total_qty, quantity_t order_qty) {
                if (agg_residual_qty >= level_total_qty) {
                    return order_qty;
                }
                // no round, any other restriction like lower boundary 2??
                return order_qty * agg_residual_qty / level_total_qty;
            };

            quantity_t level_alloc_qty = 0;
            while (order) {
                const auto order_alloc_qty = calc_allocation(agg_residual_qty, level_total_qty, order->quantity);
                if (order_alloc_qty > 0) {
                    level_alloc_qty += order_alloc_qty;

                    resting_book.modify_order(order->id, order->price, order->quantity - order_alloc_qty);
                    result.matching_orders.push_back(order_t{
                        .side     = order->side,
                        .id       = order->id,
                        .price    = order->price,
                        .quantity = order_alloc_qty,
                    });
                    result.filled_quantity += order_alloc_qty;
                } else {
                    // don't need to go through the rest orders since the order_alloc_qty will be zero for sure
                    // instead, let's apply FIFO with residual quantity starting from the front order in current level
                    // at this point -
                    // 1) current level must exist will still exist after this loop;
                    // 2) current level may have changed since beginning;
                    assert(agg_residual_qty > level_alloc_qty);

                    agg_residual_qty -= level_alloc_qty;
                    order = iter_other_side_level->second.head_order;
                    while (order) {
                        assert(order->quantity > 0);

                        // this branch must be triggered
                        if (order->quantity >= agg_residual_qty) {
                            resting_book.modify_order(order->id, order->price, order->quantity - agg_residual_qty);
                            result.matching_orders.push_back(order_t{
                                .side     = order->side,
                                .id       = order->id,
                                .price    = order->price,
                                .quantity = agg_residual_qty,
                            });
                            result.filled_quantity += agg_residual_qty;
                            agg_residual_qty = 0;
                            break;
                        }

                        result.matching_orders.push_back(order_t{
                            .side     = order->side,
                            .id       = order->id,
                            .price    = order->price,
                            .quantity = order->quantity,
                        });
                        result.filled_quantity += order->quantity;

                        agg_residual_qty -= order->quantity;

                        const auto empty_order = order;
                        order                  = order->next_order;

                        // remove empty order after updating pointer 'order'
                        resting_book.remove_order(empty_order->id);
                    }
                    assert(agg_residual_qty == 0);
                    return;
                }

                order = order->next_order;
            }

            assert(agg_residual_qty > level_alloc_qty);
            agg_residual_qty -= level_alloc_qty;
        }
    }
};

using MatchingEngine_FIFO    = matching_engine_t<matching_algo_FIFO_t, instrument_order_book_map_t>;
using MatchingEngine_ProRata = matching_engine_t<matching_algo_ProRata_t, instrument_order_book_map_t>;

void run_matching_engine_FIFO()
{
    const std::initializer_list<instrument_id_t> instruments = {10, 11, 12, 13, 14, 15};
    MatchingEngine_FIFO                          engine{instruments};
    std::array<order_t, 100>                     orders;
    for (size_t i = 0; i < orders.size(); ++i) {
        {
            const auto idx   = i;
            auto&      order = orders[idx];
            order            = order_t{
                           .side     = ((idx % 2) == 0 ? side_t::Bid : side_t::Ask),
                           .id       = idx + 1000,
                           .price    = (idx + 10) % 5 + 1,
                           .quantity = (idx + 1) % 10,
            };
            if (order.side == side_t::Bid) {
                engine.handle_order_new<side_t::Bid>(10, order.id, order.price, order.quantity);
            } else {
                engine.handle_order_new<side_t::Ask>(10, order.id, order.price, order.quantity);
            }
        }

        if (i > 20) {
            auto handle_replacement = [&engine](const auto& order) {
                if (order.side == side_t::Bid) {
                    engine.handle_order_replacement<side_t::Bid>(10, order.id, order.price, order.quantity);
                } else {
                    engine.handle_order_replacement<side_t::Ask>(10, order.id, order.price, order.quantity);
                }
            };

            const auto idx = i - 20;
            {
                // increase quantity
                auto order = orders[idx];
                order.quantity += 1;
                handle_replacement(order);
            }

            {
                // decrease quantity
                auto order = orders[idx];
                order.quantity -= 2;
                handle_replacement(order);
            }

            {
                // increase price
                auto order = orders[idx];
                order.price += 1;
                handle_replacement(order);
            }

            {
                // decrease price
                auto order = orders[idx];
                order.price -= 1;
                handle_replacement(order);
            }
        }

        if (i > 40) {
            const auto idx = i - 30;

            // cancel
            engine.handle_order_cancel(10, orders[idx].id);
        }
    }

    {
        // new order filled
        for (size_t i = 0; i < 20; ++i) {
            const auto  idx           = i;
            const auto& resting_order = orders[idx];
            const auto  order         = order_t{
                         .side     = (resting_order.side == side_t::Ask ? side_t::Bid : side_t::Ask),
                         .id       = resting_order.id + 10000,
                         .price    = resting_order.price,
                         .quantity = resting_order.quantity,    // must be full-filled
            };

            if (order.side == side_t::Bid) {
                engine.handle_order_new<side_t::Bid>(10, order.id, order.price, order.quantity);
            } else {
                engine.handle_order_new<side_t::Ask>(10, order.id, order.price, order.quantity);
            }
        }
    }

    {
        // order replacement filled
        for (size_t i = 20; i < 40; ++i) {
            const auto  idx        = i;
            const auto& next_order = orders[idx + 1];

            assert(orders[idx].side != next_order.side);

            const auto order = order_t{
                .side     = (next_order.side == side_t::Ask ? side_t::Bid : side_t::Ask),
                .id       = orders[idx].id,
                .price    = next_order.price,
                .quantity = next_order.quantity,    // must be full-filled
            };

            if (order.side == side_t::Bid) {
                engine.handle_order_new<side_t::Bid>(10, order.id, order.price, order.quantity);
            } else {
                engine.handle_order_new<side_t::Ask>(10, order.id, order.price, order.quantity);
            }
        }
    }
}

void run_matching_engine_ProRata()
{
}

#endif

