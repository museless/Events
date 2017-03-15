/*---------------------------------------------
 *     modification time: 2017.03.15 15:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: sockfd.h 
 *     creation time: 2016.11.28 15:30
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *                  Include
-*---------------------------------------------*/

#pragma once

#include "events.h"
#include <sys/socket.h>
#include <arpa/inet.h>


/*---------------------------------------------
 *                  Typedef 
-*---------------------------------------------*/

typedef struct sockaddr     Sockaddr;
typedef struct sockaddr_in  Sockin;

typedef struct sockfd       Sockfd;


/*---------------------------------------------
 *                  Typedef 
-*---------------------------------------------*/

struct sockfd {
    int32_t ev;
    Evdata  data;

    Sockin  addr;
    int32_t backlog;    /* use it for listen */
};


/*---------------------------------------------
 *                  Function
-*---------------------------------------------*/

int32_t sockfd_bind(Events *events, Sockfd *sockfd)
        __attribute__((nonnull(1, 2)));

int32_t sockfd_connect(Events *events, Sockfd *sockfd)
        __attribute__((nonnull(1, 2)));

