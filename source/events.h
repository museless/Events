/*---------------------------------------------
 *     modification time: 2016.11.25 00:00
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

#include "eventbase.h"
#include "eventsaver.h"


/*---------------------------------------------
 *            Part One: Define
-*---------------------------------------------*/

#define INF_TIMES   -1


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef struct epoll_event  Epollev;
typedef struct events       Events;


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/

struct events {
    int32_t     ep_fd;

    uint32_t    ev_maxproc; /* max process per time */
    Eventsaver  ev_saver;

    bool        ev_isstop;  /* is stop running loop */
};


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

bool    events_create(Events *events, uint32_t max_proc)
        __attribute__((nonnull(1)));

bool    events_destroy(Events *events)
        __attribute__((nonnull(1)));

bool    events_run(Events *events, int32_t times, int32_t timeout)
        __attribute__((nonnull(1)));

bool    events_ctl(Events *events, int32_t fd, 
            int32_t op, int32_t type, uint32_t event, Evdata *data)
        __attribute__((nonnull(1)));

void    events_stop_run(Events *events)
        __attribute__((nonnull(1)));

bool    eventfd_ctl(Events *events, int32_t fd, Evdata *data)
        __attribute__((nonnull(1)));

