// 
// https://www.ibm.com/support/knowledgecenter/api/content/nl/en-us/ssw_i5_54/rzab6/x1multicast.htm
//
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

#if 0
static const char*           s_group_addr    = "225.1.1.1";
static unsigned int          s_group_port    = 5555;
static const char*           s_outbound_addr = "127.0.0.1";

int main (int argc, char *argv[])
{
    /* ------------------------------------------------------------*/
    /*                                                             */
    /* Send Multicast Datagram code example.                       */
    /*                                                             */
    /* ------------------------------------------------------------*/

    /*
     * Create a datagram socket on which to send.
     */
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_fd< 0) {
        perror("opening datagram socket");
        exit(1);
    }

    /*
     * Initialize the group sockaddr structure with a
     * group address of 225.1.1.1 and port 5555.
     */
    struct sockaddr_in group_sockaddr;
    memset((char *) &group_sockaddr, 0, sizeof(group_sockaddr));
    group_sockaddr.sin_family      = AF_INET;
    group_sockaddr.sin_addr.s_addr = inet_addr(s_group_addr);
    group_sockaddr.sin_port        = htons(s_group_port);

    /*
     * Disable loopback so you do not receive your own datagrams.
     */
    char loopch = 0;
    if (setsockopt(sock_fd, IPPROTO_IP, IP_MULTICAST_LOOP,
                   (char *)&loopch,
                   sizeof(loopch)) < 0) {
        perror("setting IP_MULTICAST_LOOP:");
        close(sock_fd);
        exit(1);
    }

    /*
     * Set local interface for outbound multicast datagrams.
     * The IP address specified must be associated with a local,
     * multicast-capable interface.
     */
    struct in_addr localInterface;
    //localInterface.s_addr = inet_addr("9.5.1.1");
    localInterface.s_addr = inet_addr(s_outbound_addr);
    if (setsockopt(sock_fd, IPPROTO_IP, IP_MULTICAST_IF,
                   (char *)&localInterface,
                   sizeof(localInterface)) < 0) {
        perror("setting local interface");
        exit(1);
    }

    int  count = 0;
    char databuf[1024];
    while (1) {
        printf("Sending [%d]: ", ++count);
        memset(databuf, 0, sizeof(databuf)/sizeof(databuf[0]));
        fgets(databuf, sizeof(databuf)/sizeof(databuf[0]), stdin);
        int datalen = strlen(databuf);
        /*
         * Send a message to the multicast group specified by the
         * group_sockaddr structure.
         */
        if (sendto(sock_fd, databuf, datalen, 0,
                   (struct sockaddr*)&group_sockaddr,
                   sizeof(group_sockaddr)) < 0) {
            perror("sending datagram message");
        }
    }
    close(sock_fd);
}

#endif

