#if 0
#ifndef IPC_SERVER_C
#define IPC_SERVER_C

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <algorithm>

#define NSTRS       3                           /* no. of strings  */
#define ADDRESS     "/home/jeffery/mysocket"    /* addr to connect */

/*
 * Strings we send to the client.
 */
char *strs[NSTRS] = {
    "This is the first string from the server.\n",
    "This is the second string from the server.\n",
    "This is the third string from the server.\n"
};

void client_handler(int index, int socket_fd) {
    /*
     * We'll use stdio for reading the socket.
     */
    FILE* fp = fdopen(socket_fd, "r");

    /*
     * First we send some strings to the client.
     */
    for (int i = 0; i < NSTRS; i++)
        send(socket_fd, strs[i], strlen(strs[i]), 0);

    /*
     * Then we read some strings from the client and
     * print them out.
     */
    for (int i = 0; i < NSTRS; i++) {
        printf("[%d] ", index);
        char c;
        while ((c = fgetc(fp)) != EOF) {
            putchar(c);

            if (c == '\n')
                break;
        }
    }
}

int main()
{
    /*
     * Get a socket to work with.  This socket will
     * be in the UNIX domain, and will be a
     * stream socket.
     */
    int s;
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("server: socket");
        exit(1);
    }

    /*
     * Create the address we will be binding to.
     */
    struct sockaddr_un saun;
    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, ADDRESS);

    /*
     * Try to bind the address to the socket.  We
     * unlink the name first so that the bind won't
     * fail.
     *
     * The third argument indicates the "length" of
     * the structure, not just the length of the
     * socket name.
     */
    unlink(ADDRESS);
    int len = sizeof(saun.sun_family) + strlen(saun.sun_path);
    len = sizeof(sockaddr_un);

    if (bind(s, (struct sockaddr*)&saun, len) < 0) {
        perror("server: bind");
        exit(1);
    }

    /*
     * Listen on the socket.
     */
    if (listen(s, 5) < 0) {
        perror("server: listen");
        exit(1);
    }

    int client_index = 0;
    std::vector<std::thread> threads;
    while (1) {
        /*
         * Accept connections.  When we accept one, socket_fd
         * will be connected to the client.  fsaun will
         * contain the address of the client.
         */
        int socket_fd;
        socklen_t fromlen;
        struct sockaddr_un fsaun;
        if ((socket_fd = accept(s, (struct sockaddr*)&fsaun, &fromlen)) < 0) {
            perror("server: accept");
            exit(1);
        }

        threads.emplace_back(std::thread{client_handler, ++client_index, socket_fd});
    }

    for_each(threads.begin(), threads.end(), [](std::thread& th) { th.join(); });

    /*
     * We can simply use close() to terminate the
     * connection, since we're done with both sides.
     */
    close(s);

    exit(0);
}

#endif
#endif

