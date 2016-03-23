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
};
struct Order {
    Header  header;
    char    termination[8];
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
    int Num_Firm() {
        return header.msg_len - sizeof(price) - sizeof(quantity) - 
            sizeof(instrument) - sizeof(side) - sizeof(clientId) -
            sizeof(time_in_force) - sizeof(trader_tag) - sizeof(firm_id) -
            sizeof(termination);
    }
};
struct OrderAck : Order {
};
struct OrderFill : Order {
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
        // ...
        return entry;
    }
    //static OrderFill ParseFill(ifstream& fs, const Header& header) {
    static OrderFill ParseFill(FileReader& reader, const Header& header) {
        OrderFill entry;
        entry.header = header;
        // ...
        return entry;
    }
};

int main(int argc, char** argv) {
    if (argc < 2) return -1;

    FileReader reader;
    reader.Open(argv[1]);

    vector<Order*> orderContainer;
    try {
        //while (fs.good()) {
        while (reader.IsGood()) {
            Header header = OrderHelper::ParseHeader(reader);
            Order* pOrder = NULL;
            switch (header.msg_type) {
            case 1: //order entry
                pOrder = new OrderEntry(OrderHelper::ParseEntry(reader, header));
                break;
            case 2: //order ack
                pOrder = new OrderAck(OrderHelper::ParseAck(reader, header));
                break;
            case 3: //order fill
                pOrder = new OrderFill(OrderHelper::ParseFill(reader, header));
                break;
            default:
                cout << "unrecognized message type!" << endl;
                break;
            }
        }
    } catch (...) {
        cout << "exception happens!" << endl;
    }

    // calculate and output...
    
    // release resource
    for (unsigned int i = 0; i < orderContainer.size(); ++i) {
        delete orderContainer[i];
        orderContainer[i] = NULL;
    }
    return 0;
}

/*
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/

