#include "tcp_server.h"

tcp_server::tcp_server()
{ }

tcp_server::~tcp_server()
{ }

int tcp_server::Launch(unsigned short port) {
    int sock_server = socket(AF_INET, SOCK_STREAM, 0);
    if (ERROR == sock_server) {
        perror("server socket");
        return -1;
    }

    sockaddr_in addr_server;

    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(port);
    addr_server.sin_addr.s_addr = INADDR_ANY;
    bzero(&addr_server.sin_zero, 8);

    auto ibind = bind(sock_server, (sockaddr*)&addr_server, sizeof(sockaddr_in));
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

    communicate(sock_server);
    //std::thread th1(&tcp_server::handler, sock_server);
    //std::thread th2(&tcp_server::handler, sock_server);
    //std::thread th3(&tcp_server::handler, sock_server);
    //std::thread th4(&tcp_server::handler, sock_server);
    //th1.join();
    //th2.join();
    //th3.join();
    //th4.join();

    close(sock_server);
    printf("Socket server stops listening %s:%d\n",
          inet_ntoa(addr_server.sin_addr),
          ntohs(addr_server.sin_port));
    return 1;
}

void tcp_server::communicate(int sock_server) {
    sockaddr_in addr_client;
    unsigned int addr_size = sizeof(sockaddr_in);
    vector<std::thread> threads;
    while (1) {
        int sock_client = accept(sock_server, (sockaddr*)&addr_client, &addr_size);
        if (ERROR == sock_client) {
            perror("accept");
            return;
        }
        printf("New client connected from port %d and IP %s\n",
               ntohs(addr_client.sin_port),
               inet_ntoa(addr_client.sin_addr));

        // dump connection information
        sockaddr_in addr_info;
        getsockname(sock_server, (struct sockaddr *)&addr_info, &addr_size);
        printf("\t**getsockname - server is %s:%d\n", inet_ntoa(addr_info.sin_addr), ntohs(addr_info.sin_port));

        getsockname(sock_client, (struct sockaddr *)&addr_info, &addr_size);
        printf("\t**getsockname - connected client is %s:%d\n", inet_ntoa(addr_info.sin_addr), ntohs(addr_info.sin_port));

        getpeername(sock_server, (struct sockaddr *)&addr_info, &addr_size);
        printf("\t**getpeername - server is %s:%d\n", inet_ntoa(addr_info.sin_addr), ntohs(addr_info.sin_port));

        getpeername(sock_client, (struct sockaddr *)&addr_info, &addr_size);
        printf("\t**getpeername - connected client is %s:%d\n", inet_ntoa(addr_info.sin_addr), ntohs(addr_info.sin_port));
        // end dump

        threads.emplace_back(std::thread(&tcp_server::handleclient, sock_client));
    }
    for_each(begin(threads), end(threads), [](thread& th) {
        th.join();
    });
}
void tcp_server::handleclient(int sock_client) {
    int data_len = 1;
    char data_empty[MAX_DATA] = {0};
    char data[MAX_DATA] = {0};
    char data_resp[MAX_DATA] = {0};
    char HEAD_RESP[64] = "**Response from server**: ";
    char HEAD_REQ[64] = "**Received from client**: %s";
    while (data_len > 0) {
        data_len = recv(sock_client, data, MAX_DATA, 0);
        // empty data (data_len==1 means only has '\n') results in disconnection
        if (data_len == 1 && data[0] == '\n') break;
        if (data_len > 0) {
            strcat(data_resp, HEAD_RESP);
            strcat(data_resp, data);
            send(sock_client, data_resp, strlen(data_resp), 0);
            printf(HEAD_REQ, data);
            memcpy(data_resp, data_empty, MAX_DATA);
            memcpy(data, data_empty, MAX_DATA);
        }

        /* if server shutdown the connection, client recv returns 0
        auto r = shutdown(sock_client, SHUT_RD);
        if (r < 0) {
            printf("ERROR: shutdown err=%s\n", strerror(errno));
        } else {
            printf("Done: shutdown\n");
            break;
        }
        */
    }

    printf("Client disconnected...\n");
    close(sock_client);
}


