/*---------------------------------------------
 *     modification time: 2017.03.06 13:20
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: timerfd.h 
 *     creation time: 2016.11.24 23:35
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *                  Include
-*---------------------------------------------*/

#pragma once

#include "events.h"
#include <sys/timerfd.h>


/*---------------------------------------------
 *                  Typedef
-*---------------------------------------------*/

typedef struct timespec     Timespec;
typedef struct itimerspec   Itimerspec;


/*---------------------------------------------
 *                  Function
-*---------------------------------------------*/

int32_t timerfd_add(Events *events,
        int32_t clockid, Itimerspec *value, Evdata *data)
        __attribute__((nonnull(1, 4)));

