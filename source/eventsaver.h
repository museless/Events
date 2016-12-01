/*---------------------------------------------
 *     modification time: 2016.11.13 23:10
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
#include <unistd.h>
#include <errno.h>

#include "mempool.h"
#include "eventbase.h"


/*---------------------------------------------
 *            Part One: Define
-*---------------------------------------------*/

#define NUM_NODES   0x25


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef struct eventsaver   Eventsaver;
typedef struct savernode    Hashnode; 
typedef struct event        Event;


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/

struct event {
    Event      *next;
    int32_t     fd;
    int32_t     ev;

    Evdata      data;
};

struct savernode {
    Event      *head;
};

struct eventsaver {
    Mempool     mem;
    MATOS       lock;
    Hashnode    nodes[NUM_NODES];
};


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

bool        eventsaver_create(Eventsaver *saver)
            __attribute__((nonnull(1)));

bool        eventsaver_destroy(Eventsaver *saver)
            __attribute__((nonnull(1)));

Event      *eventsaver_add(Eventsaver *saver,
                int32_t fd, int32_t ev, Evdata *data)
            __attribute__((nonnull(1, 4)));

bool        eventsaver_delete(Eventsaver *saver, int32_t fd)
            __attribute__((nonnull(1)));

Event      *eventsaver_search(Eventsaver *saver, int32_t fd)
            __attribute__((nonnull(1)));

