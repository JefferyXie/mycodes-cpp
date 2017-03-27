#if 0

#ifndef IPC_CLIENT_EPOLL_C
#define IPC_CLIENT_EPOLL_C

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <chrono>

//
// https://users.cs.cf.ac.uk/Dave.Marshall/C/node28.html
// unix domain sockets vs. internet sockets - 
// https://lists.freebsd.org/pipermail/freebsd-performance/2005-February/001143.html
//
//
#define NSTRS       3                           /* no. of strings  */
#define ADDRESS     "/home/jeffery/mysocket"    /* addr to connect */

/*
 * Strings we send to the server.
 */
char *strs[NSTRS] = {
    "This is the first string from the client.\n",
    "This is the second string from the client.\n",
    "This is the third string from the client.\n"
};

int main()
{
    char c;
    FILE *fp;
    int i, s, len;
    struct sockaddr_un saun;

    /*
     * Get a socket to work with.  This socket will
     * be in the UNIX domain, and will be a
     * stream socket.
     */
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("client: socket");
        exit(1);
    }

    /*
     * Create the address we will be connecting to.
     */
    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, ADDRESS);

    /*
     * Try to connect to the address.  For this to
     * succeed, the server must already have bound
     * this address, and must have issued a listen()
     * request.
     *
     * The third argument indicates the "length" of
     * the structure, not just the length of the
     * socket name.
     */
    len = sizeof(saun.sun_family) + strlen(saun.sun_path);

    if (connect(s, (struct sockaddr*)&saun, len) < 0) {
        perror("client: connect");
        exit(1);
    }

    /*
     * First we send some strings to the server.
     */
    for (i = 0; i < NSTRS; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        send(s, strs[i], strlen(strs[i]), 0);
    }

    /*
     * We'll use stdio for reading the socket.
     */
    fp = fdopen(s, "r");

    /*
     * Then we read some strings from the server
     * and print them out.
     */
    while ((c = fgetc(fp)) != EOF) {
        putchar(c);
    }

    /*
     * We can simply use close() to terminate the
     * connection, since we're done with both sides.
     */
    close(s);

    exit(0);
}

#endif
#endif

