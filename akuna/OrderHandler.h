#ifndef ORDERHANDLER_H
#define ORDERHANDLER_H

#include "./include.h"
#include "./Protocol.h"
#include "./FileReader.h"

class OrderHandler {
private:
    FileReader _reader;
    vector<OrderEntry>  _entryContainer;
    vector<OrderAck>    _ackContainer;
    vector<OrderFill>   _fillContainer;
    map<uint32_t, pair<OrderFill, OrderEntry> > _mapFillEntry;
public:
    OrderHandler(char* file) {
        _reader.Open(file);
    }
    bool Parse() {
        try {
            while (_reader.IsGood()) {
                Header header = parseHeader();
                switch (header.msg_type) {
                case 1: //order entry
                    {
                        auto order = parseEntry(header);
                        if (order.IsValid()) _entryContainer.push_back(order);
                    }
                    break;
                case 2: //order ack
                    {
                        auto order = parseAck(header);
                        if (order.IsValid()) _ackContainer.push_back(order);
                    }
                    break;
                case 3: //order fill
                    {
                        auto order = parseFill(header);
                        if (order.IsValid()) _fillContainer.push_back(order);
                    }
                    break;
                default:
                    break;
                }
            }
        } catch (...) {
            cout << "exception happens!" << endl;
            return false;
        }
        return true;
    }
    string AnswerQ1() {
        auto num_entry = _entryContainer.size();
        auto num_ack = _ackContainer.size();
        auto num_fill = _fillContainer.size();
        auto num_all = num_entry + num_ack + num_fill;
        return to_string(num_all);
    }
    string AnswerQ2() {
        return to_string(_entryContainer.size()) + ", " +
            to_string(_ackContainer.size()) + ", " +
            to_string(_fillContainer.size());
    }
    string AnswerQ3() {
        // question 3)
        buidMapFillEntry();
        map<string, uint32_t> activeTrader;
        pair<string, uint32_t> activestTrader;
        for (auto it = _mapFillEntry.begin(); it != _mapFillEntry.end(); ++it) {
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
        return activestTrader.first;
    }
    string AnswerQ4() {
        // question 4)
        map<string, uint32_t> liquidityTrader;
        pair<string, uint32_t> liquiditiestTrader;
        for (auto itAck = _ackContainer.begin(); itAck != _ackContainer.end(); ++itAck) {
            const OrderAck& orderAck = *itAck;
            if (orderAck.order_status != 1) continue;
            auto itEntry = findEntryByClientId(orderAck.clientId);
            if (itEntry == _entryContainer.end() || itEntry->time_in_force != 2) continue;
            string trader(itEntry->trader_tag, sizeof(itEntry->trader_tag));
            uint32_t quant = liquidityTrader[trader] + itEntry->quantity;
            liquidityTrader[trader] = quant;
            if (liquiditiestTrader.second < quant) {
                liquiditiestTrader.first = trader;
                liquiditiestTrader.second = quant;
            }
        }
        return liquiditiestTrader.first;
    }
    string AnswerQ5() {
        // question 5)
        buidMapFillEntry();
        map<string, uint32_t> volumes;
        map<string, uint32_t> volumesBuy;
        map<string, uint32_t> volumesSell;
        for (auto it = _mapFillEntry.begin(); it != _mapFillEntry.end(); ++it) {
            const OrderFill& orderFill = it->second.first;
            const OrderEntry& orderEntry = it->second.second;
            string instrument(orderEntry.instrument, sizeof(orderEntry.instrument));
            volumes[instrument] += orderFill.fill_quantity;
            if (orderEntry.side == 1) { // buy
                volumesBuy[instrument] += orderFill.fill_quantity;
            } else if (orderEntry.side == 2) { // sell
                volumesSell[instrument] += orderFill.fill_quantity;
            }
        }
        string strVolumes;
        for (auto it = volumes.begin(); it != volumes.end(); ++it) {
            strVolumes += it->first + " : " + to_string(it->second) + " ";
        }
        return strVolumes;
    }

private:
    Header parseHeader() {
        Header header;
        _reader.Read(header.marker);
        _reader.Read(header.msg_type);
        _reader.Read(header.sequence_id);
        _reader.Read(header.timestamp);
        _reader.Read(header.msg_direction);
        _reader.Read(header.msg_len);
        return header;
    }
    void parseTermination(Order& order) {
        _reader.Read(order.termination);
    }
    OrderEntry parseEntry(const Header& header) {
        OrderEntry entry;
        entry.header = header;
        _reader.Read(entry.price);
        _reader.Read(entry.quantity);
        _reader.Read(entry.instrument);
        _reader.Read(entry.side);
        _reader.Read(entry.clientId);
        _reader.Read(entry.time_in_force);
        _reader.Read(entry.trader_tag);
        _reader.Read(entry.firm_id);
        _reader.Read(entry.firm, entry.Num_Firm());
        parseTermination(entry);
        return entry;
    }
    OrderAck parseAck(const Header& header) {
        OrderAck entry;
        entry.header = header;
        _reader.Read(entry.order_id);
        _reader.Read(entry.clientId);
        _reader.Read(entry.order_status);
        _reader.Read(entry.reject_code);
        parseTermination(entry);
        return entry;
    }
    OrderFill parseFill(const Header& header) {
        OrderFill entry;
        entry.header = header;
        _reader.Read(entry.order_id);
        _reader.Read(entry.fill_price);
        _reader.Read(entry.fill_quantity);
        _reader.Read(entry.no_of_contras);
        auto num = entry.no_of_contras;
        while (num--) {
            OrderFill::CounterParty party;
            _reader.Read(party.firm_id);
            _reader.Read(party.trader_tag);
            _reader.Read(party.quantity);
            entry.parties.push_back(party);
        }
        parseTermination(entry);
        return entry;
    }
    auto findAckByOrderId(uint32_t id) -> decltype(_ackContainer.begin()) {
        for (auto it = _ackContainer.begin(); it != _ackContainer.end(); ++it) {
            if (it->order_id == id) return it;
        }
        return _ackContainer.end();
    }
    auto findEntryByClientId(uint64_t id) -> decltype(_entryContainer.begin()) {
        for (auto it = _entryContainer.begin(); it != _entryContainer.end(); ++it) {
            if (it->clientId == id) return it;
        }
        return _entryContainer.end();
    }
    void buidMapFillEntry() {
        if (_mapFillEntry.size() > 0) return;
        for (auto itFill = _fillContainer.begin(); itFill != _fillContainer.end(); ++itFill) {
            auto itAck = findAckByOrderId(itFill->order_id);
            if (itAck == _ackContainer.end()) continue;
            auto itEntry = findEntryByClientId(itAck->clientId);
            if (itEntry == _entryContainer.end()) continue;
            _mapFillEntry.insert(make_pair(itFill->order_id, make_pair(*itFill, *itEntry)));
        }
    }
};

#endif

