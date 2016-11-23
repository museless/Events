/*---------------------------------------------
 *     modification time: 2016.11.23 13:30
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: eventbase.h 
 *     creation time: 2016.11.23 13:30
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Enum
 *       Part Three: Typedef
 *       Part Four:  Struct
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
 *               Part Two: Enum 
-*---------------------------------------------*/

enum EVTYPE {
    EVMIN = 0,

    EVREAD = EVMIN,
    EVWRITE,
    EVERROR,

    EVMAX = EVERROR,
};


/*---------------------------------------------
 *            Part Three: Typedef
-*---------------------------------------------*/

typedef void (*ev_handle)(int32_t fd, void *args);

typedef struct evdata   Evdata;


/*---------------------------------------------
 *            Part Four: Struct 
-*---------------------------------------------*/

struct evdata {
    ev_handle   handle;
    void       *args;
};


