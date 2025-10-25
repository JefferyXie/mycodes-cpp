#include "socket_common.h"

class tcp_client
{
public:
    tcp_client();
    virtual ~tcp_client();

    int Launch(const char* addr = "127.0.0.1", unsigned short port = 10000);
};

