#include "../main/header.h"
#include "xsocketserver.h"
#include "xsocketclient.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return -1;

    if (strcmp(argv[1], "server") == 0 ||
        strcmp(argv[1], "s") == 0) {
        XSocketServer srv;
        srv.Launch();
    } else if (strcmp(argv[1], "client") == 0 ||
                strcmp(argv[1], "c") == 0) {
        XSocketClient client;
        client.Launch();
    }
    return 0;
}

