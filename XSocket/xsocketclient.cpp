#include "xsocketclient.h"

XSocketClient::XSocketClient()
{
}

XSocketClient::~XSocketClient()
{
}

int XSocketClient::Launch(const char* addr, unsigned short port)
{
    int sock_client = socket(AF_INET, SOCK_STREAM, 0);
    if (ERROR == sock_client)
    {
        perror("client socket: ");
        return -1;
    }

    sockaddr_in addr_server;
    unsigned int addr_size = sizeof(sockaddr_in);

    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(port);
    addr_server.sin_addr.s_addr = inet_addr(addr);
    bzero(&addr_server.sin_zero, 8);

    auto iconnect = connect(sock_client, (sockaddr*)&addr_server, addr_size);
    if (ERROR == iconnect)
    {
        perror("connect: ");
        return -1;
    }

    while (1)
    {
        char data_req[MAX_DATA] = {0};
        char data_resp[MAX_DATA] = {0};
        fgets(data_req, MAX_DATA, stdin);
        send(sock_client, data_req, strlen(data_req), 0);
        int len = recv(sock_client, data_resp, MAX_DATA, 0);
        data_resp[len] = '\0';
        printf("%s\n", data_resp);
    }
    close(sock_client);
    return 1;
}
