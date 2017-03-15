/*---------------------------------------------
 *     modification time: 2017.03.15 15:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: sockfd.c
 *     creation time: 2016.11.28 15:15
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *                 Include
-*---------------------------------------------*/

#include "sockfd.h"


/*---------------------------------------------
 *                 Define
-*---------------------------------------------*/

#define ADDRLEN sizeof(Sockaddr)


/*---------------------------------------------
 *                 Function 
-*---------------------------------------------*/

/*-----sockfd_bind-----*/
int32_t sockfd_bind(Events *events, Sockfd *sockfd)
{
    int32_t sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
        return  -1;

    int32_t opt = 1;

    opt = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int32_t));

    if (opt == -1) {
        close(sock);
        return  -1;
    }

    if (bind(sock, (const Sockaddr *)(&sockfd->addr), ADDRLEN) == -1) {
        close(sock);
        return  -1;
    }

    if (listen(sock, sockfd->backlog) == -1) {
        close(sock);
        return  -1;
    }

    if (!events_ctl(events, sock, EPOLL_CTL_ADD, sockfd->ev, &sockfd->data)) {
        close(sock);
        return  -1;
    }

    return  sock;
}


/*-----sockfd_connect-----*/
int32_t sockfd_connect(Events *events, Sockfd *sockfd)
{
    int32_t sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
        return  -1;

    if (connect(sock, (const Sockaddr *)(&sockfd->addr), ADDRLEN) == -1) {
        close(sock);
        return  -1;
    }

    if (!events_ctl(events, sock, EPOLL_CTL_ADD, sockfd->ev, &sockfd->data)) {
        close(sock);
        return  -1;
    }

    return  sock;
}

