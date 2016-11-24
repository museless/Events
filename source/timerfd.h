/*---------------------------------------------
 *     modification time: 2016.11.24 23:35
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: timerfd.h 
 *     creation time: 2016.11.24 23:35
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

#include "events.h"
#include <sys/timerfd.h>


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef struct timespec     Timespec;
typedef struct itimerspec   Itimerspec;


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

int32_t _timerfd_add(Events *events,
        int32_t clockid, Itimerspec *value, Evdata *data)
        __attribute__((nonnull(1, 4)));

