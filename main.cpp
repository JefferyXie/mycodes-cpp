//#include <gtest/gtest.h>
//#include "header.h"
//#include "./test/Test_language.h"
//#include "./test/Test_algorithm.h"
//#include "./test/Test_multithreading.h"

#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>      // std::fstream
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

//
// googletest articles:
// http://stackoverflow.com/questions/13513905/how-to-setup-googletest-as-a-shared-library-on-linux
// http://www.ibm.com/developerworks/aix/library/au-googletestingframework.html
// http://www.yolinux.com/TUTORIALS/Cpp-GoogleTest.html
// https://github.com/google/googletest/blob/master/googletest/docs/FAQ.md#why-is-it-not-recommended-to-install-a-pre-compiled-copy-of-google-test-for-example-into-usrlocal

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

class FileReader {
public:
    ~FileReader() {
        if (_fs.is_open()) _fs.close();
    }
    bool Open(char* file) {
        _fs.open(file);
        return _fs.is_open();
    }
    template<class T>
    bool Read(T& buf) {
        return Read(buf, sizeof(buf));
    }
    template<class T>
    bool Read(T& buf, int length) {
        if (_fs.is_open()) {
            _fs.read((char*)&buf, length);
            return IsGood();
        }
        return false;
    }
    bool IsGood() {
        return _fs.good();
    }

private:
    ifstream _fs;
};

class OrderHelper {
public:
    //static Header ParseHeader(ifstream& fs) {
    static Header ParseHeader(FileReader& reader) {
        Header header;
        reader.Read(header.marker);
        reader.Read(header.msg_type);
        reader.Read(header.sequence_id);
        reader.Read(header.timestamp);
        reader.Read(header.msg_direction);
        reader.Read(header.msg_len);
        /*
        fs.read((char*)&header.marker, sizeof(header.marker));
        fs.read((char*)&header.msg_type, sizeof(header.msg_type));
        fs.read((char*)&header.sequence_id, sizeof(header.sequence_id));
        fs.read((char*)&header.timestamp, sizeof(header.timestamp));
        fs.read((char*)&header.msg_direction, sizeof(header.msg_direction));
        fs.read((char*)&header.msg_len, sizeof(header.msg_len));
        */
        return header;
    }
    static void ParseTermination(FileReader& reader, Order& order) {
        reader.Read(order.termination);
    }
    //static OrderEntry ParseEntry(ifstream& fs, const Header& header) {
    static OrderEntry ParseEntry(FileReader& reader, const Header& header) {
        OrderEntry entry;
        entry.header = header;
        reader.Read(entry.price);
        reader.Read(entry.quantity);
        reader.Read(entry.instrument);
        reader.Read(entry.side);
        reader.Read(entry.clientId);
        reader.Read(entry.time_in_force);
        reader.Read(entry.trader_tag);
        reader.Read(entry.firm_id);
        reader.Read(entry.firm, entry.Num_Firm());
        ParseTermination(reader, entry);
        return entry;
    }
    //static OrderAck ParseAck(ifstream& fs, const Header& header) {
    static OrderAck ParseAck(FileReader& reader, const Header& header) {
        OrderAck entry;
        entry.header = header;
        reader.Read(entry.order_id);
        reader.Read(entry.clientId);
        reader.Read(entry.order_status);
        reader.Read(entry.reject_code);
        ParseTermination(reader, entry);
        return entry;
    }
    //static OrderFill ParseFill(ifstream& fs, const Header& header) {
    static OrderFill ParseFill(FileReader& reader, const Header& header) {
        OrderFill entry;
        entry.header = header;
        reader.Read(entry.order_id);
        reader.Read(entry.fill_price);
        reader.Read(entry.fill_quantity);
        reader.Read(entry.no_of_contras);
        auto num = entry.no_of_contras;
        while (num--) {
            OrderFill::CounterParty party;
            reader.Read(party.firm_id);
            reader.Read(party.trader_tag);
            reader.Read(party.quantity);
            entry.parties.push_back(party);
        }
        ParseTermination(reader, entry);
        return entry;
    }
};

int main(int argc, char** argv) {
    if (argc < 2) return -1;

    FileReader reader;
    reader.Open(argv[1]);

    vector<OrderEntry> entryContainer;
    vector<OrderAck> ackContainer;
    vector<OrderFill> fillContainer;
    try {
        while (reader.IsGood()) {
            Header header = OrderHelper::ParseHeader(reader);
            switch (header.msg_type) {
            case 1: //order entry
                {
                    auto order = OrderHelper::ParseEntry(reader, header);
                    if (order.IsValid()) entryContainer.push_back(order);
                }
                break;
            case 2: //order ack
                {
                    auto order = OrderHelper::ParseAck(reader, header);
                    if (order.IsValid()) ackContainer.push_back(order);
                }
                break;
            case 3: //order fill
                {
                    auto order = OrderHelper::ParseFill(reader, header);
                    if (order.IsValid()) fillContainer.push_back(order);
                }
                break;
            default:
                break;
            }
        }
    } catch (...) {
        cout << "exception happens!" << endl;
    }

    auto findAckByOrderId = [&ackContainer](uint32_t id) {
        for (auto it = ackContainer.begin(); it != ackContainer.end(); ++it) {
            if (it->order_id == id) return it;
        }
        return ackContainer.end();
    };
    auto findEntryByClientId = [&entryContainer](uint64_t id) {
        for (auto it = entryContainer.begin(); it != entryContainer.end(); ++it) {
            if (it->clientId == id) return it;
        }
        return entryContainer.end();
    };

    // map between fill and entry by order id
    map<uint32_t, pair<OrderFill, OrderEntry> > mapFillEntry;
    for (auto itFill = fillContainer.begin(); itFill != fillContainer.end(); ++itFill) {
        auto itAck = findAckByOrderId(itFill->order_id);
        if (itAck == ackContainer.end()) continue;
        auto itEntry = findEntryByClientId(itAck->clientId);
        if (itEntry == entryContainer.end()) continue;
        mapFillEntry.insert(make_pair(itFill->order_id, make_pair(*itFill, *itEntry)));
    }

    // question 3)
    map<string, uint32_t> activeTrader;
    pair<string, uint32_t> activestTrader;
    for (auto it = mapFillEntry.begin(); it != mapFillEntry.end(); ++it) {
        const OrderFill& orderFill = it->second.first;
        const OrderEntry& orderEntry = it->second.second;
        for (auto itContra = orderFill.parties.begin(); itContra != orderFill.parties.end(); ++itContra) {
            string trader(itContra->trader_tag, sizeof(itContra->trader_tag));
            uint32_t quant = activeTrader[trader] + itContra->quantity;
            activeTrader[trader] = quant;
            if (activestTrader.second < quant) {
                activestTrader.first = trader;
                activestTrader.second = quant;
            }
        }

        // can reuse the codes in above loop...
        string trader(orderEntry.trader_tag, sizeof(orderEntry.trader_tag));
        uint32_t quant = activeTrader[trader] + orderFill.fill_quantity;
        activeTrader[trader] = quant;
        if (activestTrader.second < quant) {
            activestTrader.first = trader;
            activestTrader.second = quant;
        }
    }
    cout << "most active trader: " << activestTrader.first << "," << activestTrader.second << endl;

    // question 4)
    map<string, uint32_t> liquidityTrader;
    pair<string, uint32_t> liquiditiestTrader;
    for (auto it = mapFillEntry.begin(); it != mapFillEntry.end(); ++it) {
        const OrderFill& orderFill = it->second.first;
        const OrderEntry& orderEntry = it->second.second;
        if (orderEntry.time_in_force != 2) continue;
        string trader(orderEntry.trader_tag, sizeof(orderEntry.trader_tag));
        uint32_t quant = liquidityTrader[trader] + orderFill.fill_quantity;
        liquidityTrader[trader] = quant;
        if (liquiditiestTrader.second < quant) {
            liquiditiestTrader.first = trader;
            liquiditiestTrader.second = quant;
        }
    }
    cout << "most liquidity trader: " << liquiditiestTrader.first << "," << liquiditiestTrader.second << endl;

    liquidityTrader.clear();
    pair<string, uint32_t>().swap(liquiditiestTrader);
    for (auto itAck = ackContainer.begin(); itAck != ackContainer.end(); ++itAck) {
        const OrderAck& orderAck = *itAck;
        if (orderAck.order_status != 1) continue;
        auto itEntry = findEntryByClientId(orderAck.clientId);
        if (itEntry == entryContainer.end() || itEntry->time_in_force != 2) continue;
        string trader(itEntry->trader_tag, sizeof(itEntry->trader_tag));
        uint32_t quant = liquidityTrader[trader] + itEntry->quantity;
        liquidityTrader[trader] = quant;
        if (liquiditiestTrader.second < quant) {
            liquiditiestTrader.first = trader;
            liquiditiestTrader.second = quant;
        }
    }
    cout << "most liquidity trader: " << liquiditiestTrader.first << "," << liquiditiestTrader.second << endl;

    // question 5)
    map<string, uint32_t> volumesBuy;
    map<string, uint32_t> volumesSell;
    for (auto it = mapFillEntry.begin(); it != mapFillEntry.end(); ++it) {
        const OrderFill& orderFill = it->second.first;
        const OrderEntry& orderEntry = it->second.second;
        string instrument(orderEntry.instrument, sizeof(orderEntry.instrument));
        if (orderEntry.side == 1) { // buy
            volumesBuy[instrument] += orderFill.fill_quantity;
        } else if (orderEntry.side == 2) { // sell
            volumesSell[instrument] += orderFill.fill_quantity;
        }
    }
    cout << "buy: " << volumesBuy.size() << ", sell: " << volumesSell.size() << endl;
    for (auto it = volumesBuy.begin(); it != volumesBuy.end(); ++it) {
        if (it->second != volumesSell[it->first]) {
            cout << "NOT equal trade: " << it->first 
                << ", sell " << it->second 
                << ", buy " << volumesSell[it->first] << endl;
        }
        cout << it->first << ": " << it->second << endl;
    }

    /*
    for_each(begin(entryContainer), end(entryContainer), [](OrderEntry& o) {
        cout << o.ToString() << endl;
    });
    for_each(begin(ackContainer), end(ackContainer), [](OrderAck& o) {
        cout << o.ToString() << endl;
    });
    for_each(begin(fillContainer), end(fillContainer), [](OrderFill& o) {
        cout << o.ToString() << endl;
    });*/

    return 0;
}

/*
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/

