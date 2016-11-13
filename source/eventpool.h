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


/*---------------------------------------------
 *            Part One: Define
-*---------------------------------------------*/


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef struct eventpool    Eventpool;


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/

struct eventpool {
    void   *ptr;
};


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

bool    eventpool_create(Eventpool *pool)
        __attribute__((nonnull(1)));

