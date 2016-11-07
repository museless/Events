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

#include <pthread.h>

#include "satomic.h"
#include "mempool.h"

#include "singlelist.h"


/*---------------------------------------------
 *            Part One: Define
-*---------------------------------------------*/

#define INF_TIMES   -1

/* event type */
#define NOT_EV      -1


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef bool    (*evread)(int32_t fd);
typedef bool    (*evwrite)(int32_t fd);
typedef bool    (*everr)(int32_t fd);

typedef bool    (*evsignal)(int32_t signo);

typedef struct epoll_event  Epollev;

typedef pthread_mutex_t     Mutex;

typedef struct events       Eventpool;
typedef struct event        Event;


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/

struct events {
    int32_t     ep_fd;

    uint32_t    ev_cnt;
    uint32_t    ev_maxproc;     /* max process per time */

    Mutex       ev_poollock;    /* lock pool */
    Mempool     ev_mempool;     /* memory pool */

    SingleList  ev_list;        /* list of event */
};

struct event {
    ListData   *next;
    int32_t     fd;

    evread      reader; /* function deal with read event */
    evwrite     writer; /* function deal with write event */
    everr       errer;  /* function deal with error event */

    bool        need_del;
};


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

bool    events_create(Eventpool *pool, uint32_t max_proc)
        __attribute__((nonnull(1)));

bool    events_destroy(Eventpool *pool)
        __attribute__((nonnull(1)));

bool    events_run(Eventpool *pool, int32_t times, int32_t timeout)
        __attribute__((nonnull(1)));

int32_t event_add_socket(Eventpool *pool, int32_t events, 
        evread reader, evwrite writer, everr errer)
        __attribute__((nonnull(1)));

int32_t event_add_timer(Eventpool *pool, evread reader,
        int32_t clockid, int32_t flags)
        __attribute__((nonnull(1)));

int32_t event_add_signal(Eventpool *pool, int32_t signo,
        evsignal signproc, int32_t flags)
        __attribute__((nonnull(1)));


