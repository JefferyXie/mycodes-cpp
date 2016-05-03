#include "xsocketserver.h"

XSocketServer::XSocketServer()
{ }

XSocketServer::~XSocketServer()
{ }

int XSocketServer::Launch(unsigned short port) {
    int sock_server = socket(AF_INET, SOCK_STREAM, 0);
    if (ERROR == sock_server) {
        perror("server socket");
        return -1;
    }

    sockaddr_in addr_server;
    sockaddr_in addr_client;
    unsigned int addr_size = sizeof(sockaddr_in);

    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(port);
    addr_server.sin_addr.s_addr = INADDR_ANY;
    bzero(&addr_server.sin_zero, 8);

    auto ibind = bind(sock_server, (sockaddr*)&addr_server, addr_size);
    if (ERROR == ibind) {
        perror("bind");
        return -1;
    }

    auto ls = listen(sock_server, MAX_CLIENTS);
    if (ERROR == ls) {
        perror("listen");
        return -1;
    }
    printf("Socket server starts listening %s:%d\n",
          inet_ntoa(addr_server.sin_addr),
          ntohs(addr_server.sin_port));

    while (1) {
        int sock_client = accept(sock_server, (sockaddr*)&addr_client, &addr_size);
        if (ERROR == sock_client) {
            perror("accept");
            return -1;
        }
        printf("New client connected from port %d and IP %s\n",
               ntohs(addr_client.sin_port),
               inet_ntoa(addr_client.sin_addr));

        int data_len = 1;
        char data_empty[MAX_DATA] = {0};
        char data[MAX_DATA] = {0};
        char data_resp[MAX_DATA] = {0};
        char HEAD_RESP[64] = "**Response from server**: ";
        char HEAD_REQ[64] = "**Received from client**: %s";
        while (data_len > 0) {
            data_len = recv(sock_client, data, MAX_DATA, 0);
            if (data_len > 0) {
                strcat(data_resp, HEAD_RESP);
                strcat(data_resp, data);
                send(sock_client, data_resp, strlen(data_resp), 0);
                printf(HEAD_REQ, data);
                memcpy(data_resp, data_empty, MAX_DATA);
                memcpy(data, data_empty, MAX_DATA);
            }
        }

        printf("Client disconnected...\n");
        close(sock_client);
    }
    close(sock_server);
    printf("Socket server stops listening %s:%d\n",
          inet_ntoa(addr_server.sin_addr),
          ntohs(addr_server.sin_port));
    return 1;
}

