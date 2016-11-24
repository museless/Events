/*---------------------------------------------
 *     modification time: 2016.11.24 16:25
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: signalfd.c
 *     creation time: 2016.11.24 16:25
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Seven part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Signal fd control
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "signalfd.h"


/*---------------------------------------------
 *             Part One: Define
-*---------------------------------------------*/


/*---------------------------------------------
 *         Part Three: Local function
-*---------------------------------------------*/


/*---------------------------------------------
 *         Part Four: Signal fd control
 *
 *             1. signalfd_add
 *             2. signalfd_del
 *
-*---------------------------------------------*/

/*-----signalfd_add-----*/
int32_t signalfd_add(Events *events, int32_t signum, Evdata *data)
{
    int32_t     sigfd; 
    sigset_t    set;

    sigemptyset(&set);
    sigaddset(&set, signum);

    if ((sigfd = signalfd(-1, &set, SFD_NONBLOCK | SFD_CLOEXEC)) == -1)
        return  -1;

    if (!events_ctl(events, sigfd,
            EPOLL_CTL_ADD, EVREAD, EPOLLIN | EPOLLET, data))
        return  -1;

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
        return  -1;

    return  sigfd;
}


/*-----signalfd_oper-----*/
bool signalfd_oper(Events *events, int32_t sigfd, Evdata *data)
{
    return  events_ctl(events, sigfd,
                data ? EPOLL_CTL_MOD : EPOLL_CTL_DEL, 
                EVREAD, EPOLLIN | EPOLLET, data);
}

