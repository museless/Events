/*---------------------------------------------
 *     modification time: 2016.11.24 23:30
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: timerfd.c
 *     creation time: 2016.11.24 23:30
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Timerfd control 
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "timerfd.h"


/*---------------------------------------------
 *            Part Two: Local data 
-*---------------------------------------------*/

static Itimerspec   DefaultTspec = {{0, 0}, {0, 0}};


/*---------------------------------------------
 *         Part Four: Timerfd control
 *
 *             1. timerfd_add 
 *
-*---------------------------------------------*/

/*-----timerfd_add-----*/
int32_t timerfd_add(Events *events,
        int32_t clockid, Itimerspec *value, Evdata *data)
{
    if (!value)
        value = &DefaultTspec;

    int32_t tfd = timerfd_create(clockid, TFD_NONBLOCK | TFD_CLOEXEC);

    if (tfd == -1)
        return  -1;

    if (!events_ctl(events, tfd,
            EPOLL_CTL_ADD, EVREAD, DEFEVENT, data)) {
        close(tfd);
        return  -1;
    }

    if (timerfd_settime(tfd, 0, value, NULL) == -1) {
        close(tfd);
        return  -1;
    }

    return  tfd;
}

