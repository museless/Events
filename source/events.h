/*---------------------------------------------
 *     modification time: 2016.11.06 23:00
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
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/epoll.h>

#include "mempool/mempool.h"


/*---------------------------------------------
 *            Part One: Define
-*---------------------------------------------*/

#define INF_TIMES   -1


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef bool    (*evread)(int32_t fd);
typedef bool    (*evwrite)(int32_t fd);
typedef bool    (*evhang)(int32_t fd);
typedef bool    (*everr)(int32_t fd);

typedef struct epoll_event  Epollev;

typedef struct events       Eventpool;
typedef struct event        Event;


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/

struct events {
    int32_t     ep_fd;

    uint32_t    ev_cnt;
    uint32_t    ev_maxproc;     /* max process per time */
};

struct event {
    int32_t     fd;

    evread      reader; /* function deal with read event */
    evwrite     writer; /* function deal with write event */
    evhang      hanger; /* function deal with hang up event */
    everr       errer;  /* function deal with error event */
};


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

bool    events_create(Eventpool *pool, uint32_t max_proc);
bool    events_destroy(Eventpool *pool);

bool    events_run(Eventpool *pool, int32_t times, int32_t timeout);


