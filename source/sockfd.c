/*---------------------------------------------
 *     modification time: 2016.11.28 15:15
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: sockfd.c
 *     creation time: 2016.11.28 15:15
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Sock fd control
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "sockfd.h"


/*---------------------------------------------
 *         Part Four: Sock fd control
 *
 *          1. sockfd_bind_add
 *          2. sockfd_connect_add
 *
-*---------------------------------------------*/

/*-----sockfd_bind_add-----*/
int32_t sockfd_bind_add(Events *events, int32_t type,
        int32_t ev, Sockaddr *addr, int32_t backlog, Evdata *data)
{
    int32_t sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
        return  -1;

    if (bind(sock, (const Sockaddr *)addr, sizeof(Sockaddr)) == -1) {
        close(sock);
        return  -1;
    }

    if (listen(sock, backlog) == -1) {
        close(sock);
        return  -1;
    }

    if (!events_ctl(events, sock, EPOLL_CTL_ADD, type, ev, data)) {
        close(sock);
        return  -1;
    }

    return  sock;
}


/*-----sockfd_connect_add-----*/
int32_t sockfd_connect_add(Events *events,
        int32_t type, int32_t ev, Sockaddr *addr, Evdata *data)
{
    int32_t sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
        return  -1;

    if (connect(sock, (const Sockaddr *)addr, sizeof(Sockaddr)) == -1) {
        close(sock);
        return  -1;
    }

    if (!events_ctl(events, sock, EPOLL_CTL_ADD, type, ev, data)) {
        close(sock);
        return  -1;
    }

    return  sock;
}

