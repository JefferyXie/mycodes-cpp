#include "../main/header.h"

class XSocketServer
{
public:
    XSocketServer();
    virtual ~XSocketServer();

    int Launch(unsigned short port = 10000);

private:
    static void communate(int sock_server);
    static void handleclient(int sock_client);
};

