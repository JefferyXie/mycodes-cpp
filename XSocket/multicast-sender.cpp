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
struct in_addr        localInterface;
struct sockaddr_in    groupSock;
int                   sd;
int                   datalen;
char                  databuf[1024];

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
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("opening datagram socket");
        exit(1);
    }

    /*
     * Initialize the group sockaddr structure with a
     * group address of 225.1.1.1 and port 5555.
     */
    memset((char *) &groupSock, 0, sizeof(groupSock));
    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr("225.1.1.1");
    groupSock.sin_port = htons(5555);

    /*
     * Disable loopback so you do not receive your own datagrams.
     */
    {
        char loopch=0;

        if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP,
                    (char *)&loopch, sizeof(loopch)) < 0) {
            perror("setting IP_MULTICAST_LOOP:");
            close(sd);
            exit(1);
        }
    }

    /*
     * Set local interface for outbound multicast datagrams.
     * The IP address specified must be associated with a local,
     * multicast-capable interface.
     */
    //localInterface.s_addr = inet_addr("9.5.1.1");
    localInterface.s_addr = inet_addr("127.0.0.1");
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF,
                (char *)&localInterface,
                sizeof(localInterface)) < 0) {
        perror("setting local interface");
        exit(1);
    }

    int count = 0;
    while (1) {
        printf("Sending [%d]: ", ++count);
        memset(databuf, 0, sizeof(databuf)/sizeof(databuf[0]));
        fgets(databuf, sizeof(databuf)/sizeof(databuf[0]), stdin);
        datalen = strlen(databuf);
        /*
         * Send a message to the multicast group specified by the
         * groupSock sockaddr structure.
         */
        if (sendto(sd, databuf, datalen, 0,
                    (struct sockaddr*)&groupSock,
                    sizeof(groupSock)) < 0)
        {
            perror("sending datagram message");
        }
    }
    close(sd);
}

#endif

