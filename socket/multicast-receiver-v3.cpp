// 
// http://www.tack.ch/multicast/
//
// Multicast Server
// written for LINUX
// Version 0.0.2
//
// Change: IP_MULTICAST_LOOP : Enable / Disable loopback for outgoing messages
// 
// Compile : gcc -o server server.c
//
// This code has NOT been tested
// 

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

#define MAXBUFSIZE    65536 // Max UDP Packet size is 64 Kbyte
const char*           s_group_addr    = "226.0.0.1";
unsigned int          s_group_port    = 4096;
const char*           s_outbound_addr = "127.0.0.1";

#if 1
int main()
{
    // open a UDP socket
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0) {
        perror("Error creating socket"), exit(0);
    }

    //
    // 
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    saddr.sin_family      = PF_INET;
    saddr.sin_port        = htons(4096);        // listen on port 4096
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);  // bind socket to any interface
    if (bind(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) > 0) {
        perror("Error binding socket to interface"), exit(0);
    }

    //
    // multicast group
    struct ip_mreq imreq;
    memset(&imreq, 0, sizeof(struct ip_mreq));
    imreq.imr_multiaddr.s_addr = inet_addr(s_group_addr);
    imreq.imr_interface.s_addr = INADDR_ANY;    // use DEFAULT interface
    imreq.imr_interface.s_addr = inet_addr("127.0.0.1");//INADDR_ANY; // use DEFAULT interface

    // JOIN multicast group on default interface
    setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
               (const void *)&imreq, sizeof(struct ip_mreq));

    socklen_t socklen = sizeof(struct sockaddr_in);
    int count = 0;
    char buffer[MAXBUFSIZE];
    while (1) {
        memset(buffer, 0, sizeof(buffer)/sizeof(buffer[0]));
        recvfrom(sock, buffer, MAXBUFSIZE, 0, 
                 (struct sockaddr *)&saddr, &socklen);
        printf("Received [%d]: %s\n", ++count, buffer);
    }

    // shutdown socket
    shutdown(sock, 2);
    // close socket
    close(sock);

    return 0;
}

#endif

