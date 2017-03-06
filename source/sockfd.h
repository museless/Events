/*---------------------------------------------
 *     modification time: 2017.03.06 13:25
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


/*---------------------------------------------
 *                  Typedef 
-*---------------------------------------------*/

typedef struct sockaddr Sockaddr;


/*---------------------------------------------
 *                  Function
-*---------------------------------------------*/

int32_t sockfd_bind_add(Events *events,
            int32_t ev, Sockaddr *addr, int32_t backlog, Evdata *data)
        __attribute__((nonnull(1, 3, 5)));

int32_t sockfd_connect_add(Events *events,
            int32_t ev, Sockaddr *addr, Evdata *data)
        __attribute__((nonnull(1, 3, 4)));

