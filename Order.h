#ifndef ORDER_H
#define ORDER_H

#include "./include.h"

struct Header {
    uint16_t    marker;
    uint8_t     msg_type;
    uint64_t    sequence_id;
    uint64_t    timestamp;
    uint8_t     msg_direction;
    uint16_t    msg_len;
    Header() {
        marker = 0;
        msg_type = 0;
        sequence_id = 0;
        timestamp = 0;
        msg_direction = 0;
        msg_len = 0;
    }
    string ToString() {
        return to_string(marker) + '\t' + 
            to_string(msg_type) + '\t' +
            to_string(sequence_id) + '\t' +
            to_string(timestamp) + '\t' +
            to_string(msg_direction) + '\t' +
            to_string(msg_len);
    }
};
struct Order {
    Header  header;
    char    termination[8];
    Order() {
        memset(termination, 0, sizeof(termination));
    }
    bool IsValid() {
        string s(termination, sizeof(termination));
        return s.compare("DBDBDBDB") == 0;
    }
    virtual string ToString() {
        return header.ToString();
    }
};
struct OrderEntry : Order {
    uint64_t    price;
    uint32_t    quantity;
    char        instrument[10];
    uint8_t     side;
    uint64_t    clientId;
    uint8_t     time_in_force;
    char        trader_tag[3];
    uint8_t     firm_id;
    char        firm[256];
    OrderEntry() {
        price = 0;
        quantity = 0;
        memset(instrument, 0, sizeof(instrument));
        side = 0;
        clientId = 0;
        time_in_force = 0;
        memset(trader_tag, 0, sizeof(trader_tag));
        firm_id = 0;
        memset(firm, 0, sizeof(firm));
    }
    int Num_Firm() {
        return header.msg_len - sizeof(price) - sizeof(quantity) - 
            sizeof(instrument) - sizeof(side) - sizeof(clientId) -
            sizeof(time_in_force) - sizeof(trader_tag) - sizeof(firm_id) -
            sizeof(termination);
    }
    string ToString() {
        return Order::ToString() + '\t' +
            to_string(price) + '\t' +
            to_string(quantity) + '\t' +
            string(instrument, sizeof(instrument)) + '\t' +
            to_string(side) + '\t' +
            to_string(clientId) + '\t' +
            to_string(time_in_force) + '\t' +
            string(trader_tag, sizeof(trader_tag)) + '\t' +
            to_string(firm_id) + '\t' +
            firm;
    }
};
struct OrderAck : Order {
    uint32_t    order_id;
    uint64_t    clientId;
    uint8_t     order_status;
    uint8_t     reject_code;
    OrderAck() {
        order_id = 0;
        clientId = 0;
        order_status = 0;
        reject_code = 0;
    }
    string ToString() {
        return Order::ToString() + '\t' +
            to_string(order_id) + '\t' +
            to_string(clientId) + '\t' +
            to_string(order_status) + '\t' +
            to_string(reject_code);
    }
};
struct OrderFill : Order {
    uint32_t    order_id;
    uint64_t    fill_price;
    uint32_t    fill_quantity;
    uint8_t     no_of_contras;
    struct CounterParty {
        uint8_t     firm_id;
        char        trader_tag[3];
        uint32_t    quantity;
        CounterParty() {
            firm_id = 0;
            memset(trader_tag, 0, sizeof(trader_tag));
            quantity = 0;
        }
    };
    vector<CounterParty>    parties;
    OrderFill() {
        order_id = 0;
        fill_price = 0;
        fill_quantity = 0;
        no_of_contras = 0;
    }
    string ToString() {
        string contras;
        for_each(parties.begin(), parties.end(), [&contras](CounterParty& p) {
            contras += to_string(p.firm_id)+","+
                    string(p.trader_tag,sizeof(p.trader_tag))+","+
                    to_string(p.quantity)+";";
        });
        return Order::ToString() + '\t' +
            to_string(order_id) + '\t' +
            to_string(fill_price) + '\t' +
            to_string(fill_quantity) + '\t' +
            contras;
    }
};

#endif

