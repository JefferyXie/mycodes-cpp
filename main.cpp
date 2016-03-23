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
class OrderHelper {
public:
    static Header ParseHeader(ifstream& fs) {
        Header header;
        fs.read((char*)&header.marker, sizeof(header.marker));
        fs.read((char*)&header.msg_type, sizeof(header.msg_type));
        fs.read((char*)&header.sequence_id, sizeof(header.sequence_id));
        fs.read((char*)&header.timestamp, sizeof(header.timestamp));
        fs.read((char*)&header.msg_direction, sizeof(header.msg_direction));
        fs.read((char*)&header.msg_len, sizeof(header.msg_len));
        return header;
    }
    static OrderEntry ParseEntry(ifstream& fs, const Header& header) {
        OrderEntry entry;
        entry.header = header;
        fs.read((char*)&entry.price, sizeof(entry.price));
        fs.read((char*)&entry.quantity, sizeof(entry.quantity));
        fs.read((char*)&entry.instrument, sizeof(entry.instrument));
        fs.read((char*)&entry.side, sizeof(entry.side));
        fs.read((char*)&entry.clientId, sizeof(entry.clientId));
        fs.read((char*)&entry.time_in_force, sizeof(entry.time_in_force));
        fs.read(entry.trader_tag, sizeof(entry.trader_tag));
        fs.read((char*)&entry.firm_id, sizeof(entry.firm_id));
        fs.read(entry.firm, entry.Num_Firm());
        return entry;
    }
    static OrderAck ParseAck(ifstream& fs, const Header& header) {
        OrderAck entry;
        entry.header = header;
        // ...
        return entry;
    }
    static OrderFill ParseFill(ifstream& fs, const Header& header) {
        OrderFill entry;
        entry.header = header;
        // ...
        return entry;
    }
};

int main(int argc, char** argv) {
    if (argc < 2) return -1;

    ifstream fs;
    fs.open(argv[1]);
    if (!fs.is_open()) return -1;

    vector<Order*> orderContainer;
    try {
        while (fs.good()) {
            Header header = OrderHelper::ParseHeader(fs);
            Order* pOrder = NULL;
            switch (header.msg_type) {
            case 1: //order entry
                pOrder = new OrderEntry(OrderHelper::ParseEntry(fs, header));
                break;
            case 2: //order ack
                pOrder = new OrderAck(OrderHelper::ParseAck(fs, header));
                break;
            case 3: //order fill
                pOrder = new OrderFill(OrderHelper::ParseFill(fs, header));
                break;
            default:
                cout << "unrecognized message type!" << endl;
                break;
            }
            char termination[8] = {0};
            fs.read(termination, 8);
            if (strcmp(termination, "DBDBDBDB") != 0) {
                cout << "wrong termination code!" << endl;
                throw;
            }
        }
    } catch (...) {
        cout << "exception happens!" << endl;
    }
    fs.close();

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

