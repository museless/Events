/*---------------------------------------------
 *     modification time: 2016.11.13 23:10
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: events.h 
 *     creation time: 2016.11.13 23:10
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

#include "events.h"
#include "fdhash.h"


/*---------------------------------------------
 *            Part One: Define
-*---------------------------------------------*/

#define MAX_PROC    0x100


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef struct eventpool    Eventpool;
typedef struct event        Event;


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/

struct event {
    Datanode   *next;
    int32_t     ref;
};


struct eventpool {
    Fdhash  hash;
    Events  events;

    int32_t epfd;
};


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

bool    eventpool_create(Eventpool *pool)
        __attribute__((nonnull(1)));

