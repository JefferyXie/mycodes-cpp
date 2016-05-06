//
// http://ntrg.cs.tcd.ie/undergrad/4ba2/multicast/antony/example.html
/*
 * listener.c -- joins a multicast group and echoes all data it receives from
 *		the group to its stdout...
 *
 * Antony Courtney,	25/11/94
 * Modified by: Frédéric Bastien (25/03/04)
 * to compile without warning and work correctly
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE 256

int main(int argc, char *argv[])
{
    int fd, nbytes;
    char msgbuf[MSGBUFSIZE];

    u_int yes=1;            /*** MODIFICATION TO ORIGINAL */

    /* create what looks like an ordinary UDP socket */
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
        perror("socket");
    }

    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
        perror("Reusing ADDR failed");
    }

    /* set up destination address */
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
    addr.sin_port=htons(HELLO_PORT);

    /* bind to receive address */
    if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
        perror("bind");
    }

    /* use setsockopt() to request that the kernel join a multicast group */
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr=inet_addr(HELLO_GROUP);
    mreq.imr_interface.s_addr=inet_addr("127.0.0.1");
    mreq.imr_interface.s_addr=INADDR_ANY;
    if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
        perror("setsockopt");
    }

    /* now just enter a read-print loop */
    while (1) {
        socklen_t addrlen=sizeof(addr);
        if ((nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0, 
                (struct sockaddr *)&addr, &addrlen)) < 0) {
            perror("recvfrom");
        }
        puts(msgbuf);
    }
    return 0;
}


