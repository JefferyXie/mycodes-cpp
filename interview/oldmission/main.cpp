#include "strategy.h"

void print_help() {
    std::cout << "\n=== Old Mission Capital Similuated Trading Algo ===\n\n"
              << "Author:\tJeffery Xie (jeffery.xie@gmail.com)\n"
              << "Date:\t05/06/2019\n\n"
              << "Command parameters -\n"
              << " -symbol IBM\n"
              << " -side B\n"
              << " -max_per_order 10\n"
              << " -vwap_period 20\n"
              << " -order_timeout 10\n"
              << " -mkt 127.0.0.1:31000\n"
              << " -order 127.0.0.1:31001\n"
              << " -h (show this help)\n\n"
              << "Example:\n"
              << "./main_exe -symbol IBM -side B -max_per_order 10 "
              << "-vwap_period 5 -order_timeout 5 "
              << "-mkt 127.0.0.1:31000 -order 127.0.0.1:31001\n"
              << std::endl;
}

// ./main_exe -symbol IBM -side B -max_per_order 10 -vwap_period 5 -order_timeout 5 -mkt 127.0.0.1:31000 -order 127.0.0.1:31001
int main(int argc, char *argv[]) {
    if (argc == 2 && std::string(argv[1]) == "-h") {
        print_help();
        return 0;
    }

    std::string symbol = "IBM";
    char side = 'B';
    unsigned max_order_qty = 10;
    unsigned vwap_period_sec = 5;
    unsigned order_timeout_sec = 5;
    std::string ip_port_mkt = "127.0.0.1:31000";
    std::string ip_port_order = "127.0.0.1:31001";

    for (int i = 1; i < argc; i += 2) {
        std::string name = argv[i];
        std::string value = argv[i+1];
        if (name == "-symbol") {
            symbol = value;
        } else if (name == "-side") {
            side = value[0];
        } else if (name == "-max_per_order") {
            max_order_qty = std::stoul(value);
        } else if (name == "-vwap_period") {
            vwap_period_sec = std::stoul(value);
        } else if (name == "-order_timeout") {
            order_timeout_sec = std::stoul(value);
        } else if (name == "-mkt") {
            ip_port_mkt = value;
        } else if (name == "-order") {
            ip_port_order = value;
        } else {
            print_help();
            return 0;
        }
    }

    char buf[512] = {'\0'};
    snprintf(buf, sizeof(buf),
             "=== Strategy will use following parameters ===\n"
             "symbol:\t\t\t%s\n"
             "side:\t\t\t%c\n"
             "max_per_order:\t\t%u\n"
             "vwap_period:\t\t%u\n"
             "order_timeout:\t\t%u\n"
             "ip:port(market):\t%s\n"
             "ip:port(order):\t\t%s\n",
             symbol.c_str(),
             side,
             max_order_qty,
             vwap_period_sec,
             order_timeout_sec,
             ip_port_mkt.c_str(),
             ip_port_order.c_str());
    std::cout << buf << std::endl;


    strategy_t strat;
    strat.make_order(symbol.c_str(),
                     side,
                     max_order_qty,
                     vwap_period_sec,
                     order_timeout_sec,
                     ip_port_mkt.c_str(),
                     ip_port_order.c_str());

    return 0;
}

