#include "../main/header.h"

class XSocketServer
{
private:
    //int m_socket;
    //sockaddr_in m_addr_server;
    //sockaddr_in m_addr_client;
public:
    XSocketServer();
    virtual ~XSocketServer();

    int Launch(unsigned short port = 10000);
};

