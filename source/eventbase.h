/*---------------------------------------------
 *     modification time: 2017.03.06 13:15
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: eventbase.h 
 *     creation time: 2016.11.23 13:30
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *                  Include
-*---------------------------------------------*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/epoll.h>


/*---------------------------------------------
 *                  Define
-*---------------------------------------------*/

#define DEFEVENT    (EPOLLIN | EPOLLET)


/*---------------------------------------------
 *                 Typedef
-*---------------------------------------------*/

typedef void (*ev_handle)(int32_t fd, int32_t event, void *args);

typedef struct evdata   Evdata;


/*---------------------------------------------
 *                  Struct 
-*---------------------------------------------*/

struct evdata {
    ev_handle   handle;
    void       *args;
};


