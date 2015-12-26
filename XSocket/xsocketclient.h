#include "../header.h"

class XSocketClient
{
public:
    XSocketClient();
    virtual ~XSocketClient();

    int Launch(const char* addr = "127.0.0.1", unsigned short port = 10000);
};

