#ifndef STRATEGY_H
#define STRATEGY_H

#include "feed.h"

class strategy_t {
public:
    std::string name() { return "vwap"; }
    void stop() { _stop = true; }

    size_t make_order(const char* symbol,
                      char side,
                      unsigned max_order_qty,
                      unsigned vwap_period_sec,
                      unsigned order_timeout_sec,
                      const char* ip_port_mkt,
                      const char* ip_port_order);

private:
    void handle_trades(const std::vector<trade_t>& trades);
    void handle_quote(const quote_t& qt);
    void update_mkt_time();

    bool is_order_timeout();

    bool make_order(order_t& order);

    bool send_order(const order_t& order);

private:
    //
    // keep input paras
    std::string _para_symbol;
    char _para_side;
    unsigned _para_max_order_qty;
    unsigned _para_vwap_period_sec;
    unsigned _para_order_timeout_sec;
    std::string _para_ip_port_mkt;
    std::string _para_ip_port_order;
    // 

    bool _stop = false;
    bool _vwap_ready = false;
    unsigned _vwap = 0;
    int64_t _mkt_update_time = 0;

    feed_t _feed;
    tcp_conn_t _order_client;

    // latest bid & ask quote
    quote_t _quote;
    // latest trades within @vwap_period_sec
    std::queue<trade_t> _trades;
    uint64_t _total_trade_qty = 0;
    // orders sent out
    std::vector<order_t> _orders;
};

size_t
strategy_t::make_order(const char* symbol,
                       char side,
                       unsigned max_order_qty,
                       unsigned vwap_period_sec,
                       unsigned order_timeout_sec,
                       const char* ip_port_mkt,
                       const char* ip_port_order)
{
    _para_symbol = symbol;
    if (_para_symbol.size() > sizeof(order_t::symbol)) {
        std::cout << "[ERROR] symbol size exceeds limit "
                  << sizeof(order_t::symbol) << std::endl;
        exit(1);
    }
    _para_side = side;
    if (_para_side != 'B' && _para_side != 'S') {
        std::cout << "[ERROR] side should be 'B' or 'S' than "
                  << _para_side << std::endl;
        exit(1);
    }
    _para_max_order_qty     = max_order_qty;
    _para_vwap_period_sec   = vwap_period_sec;
    _para_order_timeout_sec = order_timeout_sec;
    _para_ip_port_mkt       = ip_port_mkt;
    _para_ip_port_order     = ip_port_order;

    if (!_order_client.connect(_para_ip_port_order)) {
        std::cout << "[ERROR] failed to init order client "
                  << _para_ip_port_order << std::endl;
        exit(1);
    }

    // keep mkt data in every cycle
    quote_t quote;
    std::vector<trade_t> trades;
    if (!_feed.init(_para_symbol, _para_ip_port_mkt,
        [&quote](quote_t&& qt) {
            quote = std::move(qt);
        },
        [&trades](trade_t&& trd) {
            trades.push_back(std::move(trd));
        }))
    {
        std::cout << "[ERROR] failed to init feed "
                  << _para_ip_port_mkt << std::endl;
        exit(1);
    }

    // this para can be configured
    constexpr unsigned FEED_PULL_TIMES_PER_CYCLE = 5;

    while (!_stop) {
        _feed.pull(FEED_PULL_TIMES_PER_CYCLE);
        handle_quote(quote);
        handle_trades(trades);
        update_mkt_time();

        order_t order;
        if (make_order(order)) {
            char buf[256] = {'\0'};
            snprintf(buf, sizeof(buf),
                     "[INFO] make order: (_vwap=%u, %s) --> %s",
                     _vwap, util::to_string(_quote).c_str(),
                     util::to_string(order).c_str());
            std::cout << buf << std::endl;
            
            send_order(order);
        }

        memset(&quote, sizeof(quote), 0);
        trades.clear();
    }

    return _orders.size();
}

void
strategy_t::handle_quote(const quote_t& qt)
{
    if (qt.ts > _quote.ts) {
        _quote = qt;
    }
}

void
strategy_t::handle_trades(const std::vector<trade_t>& trades)
{
    if (trades.size() == 0) return;

    uint64_t total_qty = _total_trade_qty;
    uint64_t prc_times_qty = _vwap * total_qty;

    auto& latest_trd = trades.back();
    while (!_trades.empty()) {
        auto& old_trd = _trades.front();
        if (latest_trd.ts - old_trd.ts >
            (int64_t)_para_vwap_period_sec*1000000000) {
            if (!_vwap_ready) {
                _vwap_ready = true;
                std::cout << "[INFO] vwap is ready!" << std::endl;
            }

            total_qty -= old_trd.qty;
            prc_times_qty -= old_trd.qty * old_trd.prc;
            _trades.pop();
        }
        else
            break;
    }

    for (auto& trd : trades) {
        total_qty += trd.qty;
        prc_times_qty += trd.qty * trd.prc;
        _trades.push(trd);
    }
    _vwap = prc_times_qty / total_qty;
    _total_trade_qty = total_qty;
}

void
strategy_t::update_mkt_time()
{
    _mkt_update_time = std::max(
        _quote.ts,
        _trades.empty() ? 0 : _trades.back().ts);
}

bool
strategy_t::is_order_timeout()
{
    if (!_orders.size()) return true;

    return ((int64_t)_para_order_timeout_sec * 1000000000 + _orders.back().ts
           < _mkt_update_time);
}

bool
strategy_t::make_order(order_t& order)
{
    if (_vwap_ready && is_order_timeout()) {
        if (_para_side == 'B' && _vwap >= _quote.ask_prc) {
            memcpy(order.symbol, _para_symbol.c_str(), _para_symbol.size());
            order.ts = _mkt_update_time;
            order.side = 'B';
            order.prc = _quote.ask_prc;
            order.qty = std::min(_quote.ask_qty, _para_max_order_qty);
            return true;
        } else if (_para_side == 'S' && _vwap <= _quote.bid_prc) {
            memcpy(order.symbol, _para_symbol.c_str(), _para_symbol.size());
            order.ts = _mkt_update_time;
            order.side = 'S';
            order.prc = _quote.bid_prc;
            order.qty = std::min(_quote.bid_qty, _para_max_order_qty);
            return true;
        }
    }
    return false;
}

bool
strategy_t::send_order(const order_t& order)
{
    if (_order_client.write((const char*)&order, sizeof(order))) {
        _orders.push_back(order);

        char buf[256] = {'\0'};
        snprintf(buf, sizeof(buf),
                "[INFO] sent order (%lu): %s",
                 _orders.size(),
                 util::to_string(order).c_str());
        std::cout << buf << std::endl;

        return true;
    }
    return false;
}

#endif

