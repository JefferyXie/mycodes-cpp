#include "../main/header.h"

class tcp_server
{
public:
    tcp_server();
    virtual ~tcp_server();

    int Launch(unsigned short port = 10000);

private:
    static void communate(int sock_server);
    static void handleclient(int sock_client);
};

