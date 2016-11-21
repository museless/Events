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


/*---------------------------------------------
 *            Part One: Define
-*---------------------------------------------*/

#define INF_TIMES   -1

enum EVTYPE {
    EVMIN = 0,

    EVREAD = EVMIN,
    EVWRITE,
    EVERROR,

    EVMAX = EVERROR,
};


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef bool (*ev_functor)(void *object, int32_t fd, uint8_t type);

typedef struct epoll_event  Epollev;
typedef struct events       Events;


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/

struct events {
    int32_t     ep_fd;

    uint32_t    ev_maxproc; /* max process per time */
    ev_functor  ev_func;

    void       *ev_obj;
};


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

int32_t events_create(Events *events, uint32_t max_proc, 
            void *evobj, ev_functor functor)
        __attribute__((nonnull(1, 3, 4)));

bool    events_destroy(Events *events)
        __attribute__((nonnull(1)));

bool    events_run(Events *events, int32_t times, int32_t timeout)
        __attribute__((nonnull(1)));

