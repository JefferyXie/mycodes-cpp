// 
// http://www.tack.ch/multicast/
//
// Multicast Client
// written for LINUX
// Version 0.0.2
//
// Change: IP_MULTICAST_LOOP : Enable / Disable loopback for outgoing messages
// 
// Compile : gcc -o client client.c
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
static const char*           s_group_addr    = "226.0.0.1";
static unsigned int          s_group_port    = 4096;
static const char*           s_outbound_addr = "127.0.0.1";

#if 0
int main()
{
    // open a UDP socket
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error creating socket"), exit(0);
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(struct sockaddr_in));

    // what is this bind for?
    saddr.sin_family      = PF_INET;
    saddr.sin_port        = htons(0);           // Use the first free port
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);  // bind socket to any interface
    if (bind(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) < 0) {
        perror("Error binding socket to interface"), exit(0);
    }

    //
    // multicast group
    
    // set destination multicast address
    saddr.sin_family      = PF_INET;
    saddr.sin_addr.s_addr = inet_addr(s_group_addr);
    saddr.sin_port        = htons(s_group_port);

    // Set multicast packet TTL to 3; default TTL is 1
    unsigned char ttl = 3;
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
               &ttl, sizeof(unsigned char));

    // send multicast traffic to myself too
    unsigned char one = 1;
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP,
               &one, sizeof(unsigned char));

    //
    // outgoing interface
    struct in_addr iaddr;
    memset(&iaddr, 0, sizeof(struct in_addr));
    iaddr.s_addr = INADDR_ANY; // use DEFAULT interface
    iaddr.s_addr = inet_addr("127.0.0.1"); 

    // Set the outgoing interface to DEFAULT
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF,
               &iaddr, sizeof(struct in_addr));

    int count = 0;
    char buffer[MAXBUFSIZE];
    while (1) {
        printf("Sending [%d]: ", ++count);
        memset(buffer, 0, sizeof(buffer)/sizeof(buffer[0]));
        fgets(buffer, sizeof(buffer)/sizeof(buffer[0]), stdin);
        sendto(sock, buffer, strlen(buffer), 0,
               (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    }

    // shutdown socket
    shutdown(sock, 2);
    // close socket
    close(sock);

    return 0;
}

#endif

