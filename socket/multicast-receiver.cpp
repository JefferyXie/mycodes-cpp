//
// https://www.ibm.com/support/knowledgecenter/api/content/nl/en-us/ssw_i5_54/rzab6/x2multicast.htm
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
static const char*           s_local_addr    = "127.0.0.1";

int main (int argc, char *argv[])
{

    /* ------------------------------------------------------------*/
    /*                                                             */
    /* Receive Multicast Datagram code example.                    */
    /*                                                             */
    /* ------------------------------------------------------------*/

    /*
     * Create a datagram socket on which to receive.
     */
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_fd < 0) {
        perror("opening datagram socket");
        exit(1);
    }

    /*
     * Enable SO_REUSEADDR to allow multiple instances of this
     * application to receive copies of the multicast datagrams.
     */
    int reuse = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR,
                   (char *)&reuse,
                   sizeof(reuse)) < 0) {
        perror("setting SO_REUSEADDR");
        close(sock_fd);
        exit(1);
    }

    /*
     * Bind to the proper port number with the IP address
     * specified as INADDR_ANY.
     */
    struct sockaddr_in local_sockaddr;
    memset((char *) &local_sockaddr, 0, sizeof(local_sockaddr));
    local_sockaddr.sin_family      = AF_INET;
    // should this port match the group's port???
    local_sockaddr.sin_port        = htons(5555);
    // what else IP should be used here?
    local_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    local_sockaddr.sin_addr.s_addr = inet_addr("10.0.2.21");
//    local_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sock_fd,
             (struct sockaddr*)&local_sockaddr,
             sizeof(local_sockaddr))) {
        perror("binding datagram socket");
        close(sock_fd);
        exit(1);
    }
/*    int mc_all = 0;
    if (setsockopt(sock_fd,
                   IPPROTO_IP,
                   IP_MULTICAST_ALL,
                   (void*) &mc_all, sizeof(mc_all)) < 0) {
        perror("setsockopt() failed");
        close(sock_fd);
        exit(1);
    }
*/

    /*
     * Join the multicast group 225.1.1.1 on the local 9.5.1.1
     * interface.  Note that this IP_ADD_MEMBERSHIP option must be
     * called for each local interface over which the multicast
     * datagrams are to be received.
     */
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = inet_addr(s_group_addr);
    group.imr_interface.s_addr = inet_addr(s_local_addr);
    //group.imr_interface.s_addr = inet_addr("9.5.1.1");
    if (setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                   (void*)&group, sizeof(group)) < 0) {
        perror("adding multicast group");
        close(sock_fd);
        exit(1);
    }

    int count = 0;
    char databuf[1024];
    while (1) {
        memset(databuf, 0, sizeof(databuf)/sizeof(databuf[0]));
        /*
         * Read from the socket.
         */
        int datalen = sizeof(databuf);
        if (read(sock_fd, databuf, datalen) < 0) {
            perror("reading datagram message");
            close(sock_fd);
            exit(1);
        }
        printf("Received [%d]: %s\n", ++count, databuf);
    }
    close(sock_fd);
}

#endif

