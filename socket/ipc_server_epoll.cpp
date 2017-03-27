#if 0
#ifndef IPC_SERVER_C
#define IPC_SERVER_C

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <algorithm>

/*
 * https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/
 * 
 - The following system calls are provided to create and manage an epoll instance:

   *  epoll_create creates an epoll instance and returns a file
      descriptor referring to that instance.  (The more recent
      epoll_create1 extends the functionality of epoll_create.)

   *  Interest in particular file descriptors is then registered via
      epoll_ctl. The set of file descriptors currently registered on
      an epoll instance is sometimes called an epoll set.

   *  epoll_wait waits for I/O events, blocking the calling thread if
      no events are currently available.

 - recv/send vs. read/write:

   *  recv/send let you specify certain options for the actual operation.
      read/write are the 'universal' file descriptor functions while recv/send are
      slightly more specialized (for instance, you can set a flag to ignore SIGPIPE,
      or to send out-of-band messages...).

   *  read() is equivalent to recv() with a flags parameter of 0. Other values for the
      flags parameter change the behaviour of recv(). Similarly, write() is equivalent
      to send() with flags == 0.
 * 
 * */
#define NSTRS       3                           /* no. of strings  */
#define ADDRESS     "/home/jeffery/mysocket"    /* addr to connect */

int
make_socket_non_blocking (int socket_fd) {
    int flags = fcntl (socket_fd, F_GETFL, 0);
    if (flags == -1) {
        perror ("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    int s = fcntl (socket_fd, F_SETFL, flags);
    if (s == -1) {
        perror ("fcntl");
        return -1;
    }
    return 0;
}

bool
set_epoll_file (int set_file, int op, int epoll_file) {
    struct epoll_event event;
    int                posix_error;

    event.events   = EPOLLIN|EPOLLRDHUP|EPOLLHUP|EPOLLERR;
    event.data.fd  = set_file;
    // epoll_event.data.ptr can be used for custom data, not used in this case
    //event.data.ptr = data;

    posix_error = epoll_ctl(epoll_file, op, set_file, &event);

    if(posix_error == -1)
    {
        printf("error setepollfile: %s\n", strerror(errno));
        return false;
    }
    return true;
}

void
client_handler(int socket_fd) {
    constexpr int MAX_DATA_SIZE = 128;
    /*
     * First we send some strings to the client.
     */
    char msg_to_client[MAX_DATA_SIZE] = {0};
    sprintf(msg_to_client, "[To %d] This is the message from server.\n", socket_fd);
    send(socket_fd, msg_to_client, strlen(msg_to_client), 0);

    /*
     * Then we read some strings from the client and
     * print them out.
     */
    int total_recvd_size = 0;
    char msg_from_client[MAX_DATA_SIZE] = {0};
    while (1) {
        constexpr int CHUNK_DATA_SIZE = 8;

        char msg_chunk[CHUNK_DATA_SIZE] = {0};
        int recvd_size = recv(socket_fd,
                              msg_chunk,
                              CHUNK_DATA_SIZE,
                              0);
        if (recvd_size > 0) {
            if (recvd_size < MAX_DATA_SIZE - total_recvd_size) {
                memcpy(msg_from_client+total_recvd_size, msg_chunk, recvd_size);
                total_recvd_size += recvd_size;
            } else {
                // if data overflow, the epoll_wait will detect the incoming data and continue
                // trigger client_handler to read rest data!
                printf("[From %d, data overflow]!!!\n", socket_fd);
                break;
            }
        } else if (recvd_size == 0) {
            printf("[From %d, 0 bytes received]!!!\n", socket_fd);
            break;
        } else {
            //printf("error recv: %s\n", strerror(errno));
            break;
        }
    }
    printf("[From %d, %d bytes received] %s\n", socket_fd, total_recvd_size, msg_from_client);
}

int
main() {
    /*
     * Get a socket to work with.  This socket will
     * be in the UNIX domain, and will be a
     * stream socket.
     */
    int listen_socket;
    if ((listen_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
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

    if (bind(listen_socket, (struct sockaddr*)&saun, len) < 0) {
        perror("server: bind");
        exit(1);
    }

    /*
     * Listen on the socket.
     */
    if (listen(listen_socket, 5) < 0) {
        perror("server: listen");
        exit(1);
    }

    constexpr int MAX_EVENTS = 64;
    constexpr int WAIT_TIMEOUT_MS = 1000;

    // make socket non-blocking
    make_socket_non_blocking(listen_socket);

    // create epoll file
    int epoll_file = 0;
    epoll_file = epoll_create(MAX_EVENTS);
    set_epoll_file(listen_socket, EPOLL_CTL_ADD, epoll_file);

    while(1)
    {
        epoll_event signaled_events[MAX_EVENTS];

        int signal_count = epoll_wait(epoll_file, signaled_events, MAX_EVENTS, WAIT_TIMEOUT_MS);
        if(signal_count == -1)
        {
            if(errno == EINTR)
                continue;
            break;
        }

        while(signal_count-- > 0)
        {
            auto& event = signaled_events[signal_count];

            // Should use *.data.fd than .ptr
            //void* epoll_data = signaled_events[signal_count].data.ptr;
            int epoll_fd = signaled_events[signal_count].data.fd;

            if(event.events&(EPOLLRDHUP|EPOLLHUP))
            {
                if(epoll_fd == listen_socket)
                {
                    printf("listen_socket hangup detected, EPOLLRDHUP=%d EPOLLHUP=%d\n",
                           event.events&EPOLLRDHUP ? 1 : 0,
                           event.events&EPOLLHUP ? 1 : 0);
                }
                else
                {
                    printf("Client [%d] disconnected\n", epoll_fd);	

                    // Remove this connection from the epoll file to ensure we don't read anything
                    // else for it
                    set_epoll_file(epoll_fd, EPOLL_CTL_DEL, epoll_file);
                }
            }
            /* 
             * We have a notification on the listening socket, which
             * means one or more incoming connections.
             */
            else if(epoll_fd == listen_socket)
            {
                /*
                 * Accept connections.  When we accept one, socket_fd
                 * will be connected to the client.  fsaun will
                 * contain the address of the client.
                 */
                int socket_fd;
                socklen_t fromlen;
                struct sockaddr_un fsaun;
                if ((socket_fd = accept(listen_socket, (struct sockaddr*)&fsaun, &fromlen)) < 0) {
                    perror("server: accept");
                    exit(1);
                }
                if (socket_fd == -1) {
                    perror("accept");
                    break;
                }
                make_socket_non_blocking(socket_fd);
                set_epoll_file(socket_fd, EPOLL_CTL_ADD, epoll_file);
            }
            else
            {
                // This is where the process reads the incoming messages off of IPC
                client_handler(epoll_fd);
            }
        }
    }

    /*
     * We can simply use close() to terminate the
     * connection, since we're done with both sides.
     */
    close(listen_socket);

    exit(0);
}

#endif
#endif

