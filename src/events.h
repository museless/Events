/*---------------------------------------------
 *     modification time: 2016.11.03 14:45
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: events.h 
 *     creation time: 2016.11.03 14:45
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/epoll.h>


/*---------------------------------------------
 *            Part One: Define
-*---------------------------------------------*/

#define ENLARGE_TIMES   2


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef struct int32_t      (*evfunctor)(void);

typedef struct epoll_event  Epollev;

typedef struct events       Eventpool;
typedef struct event        Event;


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/

struct events {
    int32_t     ep_fd;

    int32_t     ep_fsize;   /* event pool initialize size */
    int32_t     ep_currsize;/* event pool current size */

    uint32_t    ev_cnt;
    Epollev    *evs;
};

struct event {
    int32_t     fd;
    evfunctor   functor;
};


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

bool    events_create(Eventpool *pool, int32_t init_size);
bool    events_destroy(Eventpool *pool);

void    events_run(Eventpool *pool, int32_t times, int32_t timeout);


