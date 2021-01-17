// 
// http://ntrg.cs.tcd.ie/undergrad/4ba2/multicast/antony/example.html
/*
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 *
 * Antony Courtney,	25/11/94
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define HELLO_PORT  12345
#define HELLO_GROUP "225.0.0.37"

#if 0
int main(int argc, char *argv[])
{
    const char *message = "Hello, World!\0";

    int fd;
    /* create what looks like an ordinary UDP socket */
    if ((fd = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
        perror("socket");
    }

    // set outgoing interface
    struct in_addr localInterface;
    memset(&localInterface, 0, sizeof(localInterface));
    localInterface.s_addr = inet_addr("127.0.0.1");
//    localInterface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF,
        &localInterface, sizeof(localInterface)) < 0) {
        perror("setting local interface");
    }

    /* set up destination address */
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr(HELLO_GROUP);
    addr.sin_port        = htons(HELLO_PORT);
    /* now just sendto() our destination! */
    while (1) {
        if (sendto(fd, message, strlen(message), 0,
            (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("sendto");
        }
        sleep(2);
    }
    return 0;
}

#endif

