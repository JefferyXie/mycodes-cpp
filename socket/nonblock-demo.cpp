#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * select / poll / epoll: practical difference for system architects
 * http://www.ulduzsoft.com/2014/01/select-poll-epoll-practical-difference-for-system-architects/
 *
 * https://www.ibm.com/support/knowledgecenter/ssw_i5_54/rzab6/xnonblock.htm
 * https://www.ibm.com/support/knowledgecenter/ssw_i5_54/rzab6/poll.htm
 * http://www.wangafu.net/~nickm/libevent-book/01_intro.html
 * */

// 
// A few use cases when doing socket:
// 1) blocking socket
//    create multiple threads to handle multiple sockets respectively
// 2) non-blocking socket
//    implemented by using fcntl() with flag O_NONBLOCK, usually cannot use this solution lonely
// 3) blocking socket + select()/poll()
//    select()/poll() can monitor sets of sockets and handle them within one thread
// 4) non-blocking socket + select()/poll()
//    Using poll() or select() with a non-blocking file descriptor gives you two advantages:
// 
//    a) You can set a timeout to block for;
//    b) You can wait for any of a set of file descriptors to become available.
//    If you only have a single file descriptor (socket) to wait for, and you don't mind 
//    waiting forever on it, then yes, you can just use a blocking call.

//    The second advantage is really the killer use case for select() and friends. It means
//    that you can handle multiple socket connections, as well as standard input and standard 
//    output and possibly file I/O, all with a single thread of control.

#if 0

#define STDIN 0  // file descriptor for standard input

// http://beej.us/guide/bgnet/output/html/multipage/advanced.html
int main(void)
{
    struct timeval tv;
    fd_set readfds;

    /* Wait up to 2.5 seconds. */
    tv.tv_sec = 2;
    tv.tv_usec = 500000;

    FD_ZERO(&readfds);
    FD_SET(STDIN, &readfds);

    // don't care about writefds and exceptfds:
    select(STDIN+1, &readfds, NULL, NULL, &tv);

    if (FD_ISSET(STDIN, &readfds))
        printf("A key was pressed!\n");
    else
        printf("Timed out.\n");

    return 0;
} 

#endif

