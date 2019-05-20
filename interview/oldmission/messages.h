#ifndef MESSAGES_H
#define MESSAGES_H

#include "header.h"

#pragma pack(push)
#pragma pack(1)

struct header_t {
    uint8_t len     = 0;
    uint8_t type    = 0; // 1: quote, 2: trade
};
struct quote_t {
    header_t hdr;
    int64_t ts      = 0; // epoch nano
    char symbol[8]  = {'\0'};
    uint32_t bid_prc = 0;
    uint32_t bid_qty = 0;
    uint32_t ask_prc = 0;
    uint32_t ask_qty = 0;
};
struct trade_t {
    header_t hdr;
    int64_t ts      = 0; // epoch nano
    char symbol[8]  = {'\0'};
    uint32_t prc    = 0;
    uint32_t qty    = 0;
};
struct order_t {
    int64_t ts      = 0; // epoch nano
    char symbol[8]  = {'\0'};
    char side       = 0; // 'B'/'S'
    uint32_t prc    = 0;
    uint32_t qty    = 0;
};

#pragma pack(pop)  

namespace util {

    static std::string to_string(const header_t& hdr) {
        char buf[256] = {'\0'};
        snprintf(buf, sizeof(buf),
                 "[header:len=%hhu type=%hhu]",
                 hdr.len, hdr.type);
        return std::string(buf);
    }

    static std::string to_string(const trade_t& trd) {
        char buf[256] = {'\0'};
        snprintf(buf, sizeof(buf),
                 "[trade:len=%hhu type=%hhu ts=%lu symbol=%s prc=%u qty=%u]",
                 trd.hdr.len, trd.hdr.type,
                 trd.ts, trd.symbol,
                 trd.prc, trd.qty);
        return std::string(buf);
    }

    static std::string to_string(const quote_t& qt) {
        char buf[256] = {'\0'};
        snprintf(buf, sizeof(buf),
                 "[quote:len=%hhu type=%hhu ts=%lu symbol=%s bid_prc=%u bid_qty=%u ask_prc=%u ask_qty=%u]",
                 qt.hdr.len, qt.hdr.type,
                 qt.ts, qt.symbol,
                 qt.bid_prc, qt.bid_qty,
                 qt.ask_prc, qt.ask_qty);
        return std::string(buf);
    }

    static std::string to_string(const order_t& order) {
        char buf[256] = {'\0'};
        snprintf(buf, sizeof(buf),
                 "[order:ts=%lu symbol=%s side=%c prc=%u qty=%u]",
                 order.ts, order.symbol,
                 order.side,
                 order.prc, order.qty);
        return std::string(buf);
    }
}

#endif
