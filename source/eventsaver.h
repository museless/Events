/*---------------------------------------------
 *     modification time: 2016.11.15 09:15
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: eventsaver.h 
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

#include "fdhash.h"
#include "events.h"


/*---------------------------------------------
 *            Part One: Define
-*---------------------------------------------*/


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef struct eventsaver   Eventsaver;
typedef struct event        Event;

typedef void (*ev_handle)(int32_t fd, void *args);


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/

struct event {
    Datanode   *next;
    int32_t     ref;
    ev_handle   handle;
    void       *args;
};


struct eventsaver {
    Fdhash  readhash;
    Fdhash  writehash;
    Fdhash  errorhash;
};


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

bool    eventsaver_create(Eventsaver *saver)
        __attribute__((nonnull(1)));

bool    eventsaver_destroy(Eventsaver *saver)
        __attribute__((nonnull(1)));

bool    eventsaver_add(Eventsaver *saver,
            uint8_t type, int32_t fd, ev_handle functor, void *args)
        __attribute__((nonnull(1)));

bool    eventsaver_delete(Eventsaver *saver, uint8_t type, int32_t fd)
        __attribute__((nonnull(1)));

Event  *eventsaver_search(Eventsaver *saver, uint8_t type, int32_t fd)
        __attribute__((nonnull(1)));


