#include "../header.h"
#include "xsocketserver.h"
#include "xsocketclient.h"

int main(int argc, char *argv[])
{
    XSocketServer srv;
    srv.Launch();
    XSocketClient client;
    client.Launch();

    int i;
    cin >> i;
    return 0;
}

