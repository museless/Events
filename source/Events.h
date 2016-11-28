/*---------------------------------------------
 *     modification time: 2016.11.28 15:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: Events.h 
 *     creation time: 2016.11.28 15:00
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
#include "timerfd.h"
#include "signalfd.h"


/*---------------------------------------------
 *             Part One: Define
-*---------------------------------------------*/

#define eventfd_ctl(events, fd, data) \
    events_ctl(events, fd, \
        data ? EPOLL_CTL_MOD : EPOLL_CTL_DEL, EVREAD, DEFEVENT, data);
