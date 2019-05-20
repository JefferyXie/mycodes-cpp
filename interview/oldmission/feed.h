#ifndef FEED_H
#define FEED_H

#include "header.h"
#include "tcp_conn.h"
#include "messages.h"

using cb_quote_t = void(quote_t&&);
using cb_trade_t = void(trade_t&&);

class feed_t {
    enum feed_mask_t : uint8_t {
        NONE    = 0,
        QUOTE   = 1 << 0,
        TRADE   = 1 << 1,
    };
public:
    feed_t() {

    }
    bool init(const std::string& symbol,
              const std::string& ip_port,
              std::function<cb_quote_t>&& cb_qt,
              std::function<cb_trade_t>&& cb_trd) {
        _symbol = symbol;
        _cb_quote = std::move(cb_qt);
        _cb_trade = std::move(cb_trd);
        return _conn.connect(ip_port);
    }

    size_t pull(unsigned max_times = 5) {
        char buf[BUF_LEN];
        size_t msg_cnt = 0;
        while (max_times--) {
            memset(buf, BUF_LEN, 0);

            // fill in the partial msg from last time
            auto partial_len = _partial_msg.size();
            memcpy(buf, _partial_msg.data(), partial_len);

            auto len = _conn.read(buf + partial_len, BUF_LEN - partial_len);

            // don't pull in current cycle if no data is available for now
            if (len == 0) break;

            _partial_msg.clear();

            unsigned pos = 0;
            while (pos < len) {
                auto pbuf = buf + pos;
                auto hdr = reinterpret_cast<header_t*>(pbuf);
                if (pos + hdr->len + sizeof(header_t) > len) {
                    // not an integral msg, keep it for future use
                    _partial_msg.assign(pbuf, len - pos);
                    break;
                } else {
                    pos += hdr->len + sizeof(header_t);
                }

                if (hdr->type == 1) {
                    quote_t qt;
                    memcpy(&qt, pbuf, hdr->len + sizeof(header_t));
                    if (_symbol == qt.symbol) {
                        _cb_quote(std::move(qt));
                        std::cout << "[DEBUG] "
                                  << util::to_string(qt) << std::endl;
                    }
                } else if (hdr->type == 2) {
                    trade_t trd;
                    memcpy(&trd, pbuf, hdr->len + sizeof(header_t));
                    if (_symbol == trd.symbol) {
                        _cb_trade(std::move(trd));
                        std::cout << "[DEBUG] "
                                  << util::to_string(trd) << std::endl;
                    }
                } else {
                    std::cout << "[WARNING] skip unknown header: "
                              << util::to_string(*hdr) << std::endl;
                }
                ++msg_cnt;
            }
        }
        return msg_cnt;
    }

private:
    static constexpr unsigned BUF_LEN = 2048;
    std::string _partial_msg;
    std::string _symbol;
    tcp_conn_t _conn;
    std::function<cb_quote_t> _cb_quote;
    std::function<cb_trade_t> _cb_trade;
};

#endif
