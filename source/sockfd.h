/*---------------------------------------------
 *     modification time: 2016.12.01 23:30
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: sockfd.h 
 *     creation time: 2016.11.28 15:30
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Typedef
 *       Part Three: Struct
 *
 *       Part Four:  Function
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#pragma once

#include "events.h"
#include <sys/socket.h>


/*---------------------------------------------
 *            Part Two: Typedef 
-*---------------------------------------------*/

typedef struct sockaddr Sockaddr;


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

int32_t sockfd_bind_add(Events *events,
            int32_t ev, Sockaddr *addr, int32_t backlog, Evdata *data)
        __attribute__((nonnull(1, 3, 5)));

int32_t sockfd_connect_add(Events *events,
            int32_t ev, Sockaddr *addr, Evdata *data)
        __attribute__((nonnull(1, 3, 4)));

