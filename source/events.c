/*---------------------------------------------
 *     modification time: 2016.11.06 00:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: events.c
 *     creation time: 2016.11.03 15:05
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Eight part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Eventpool open close api
 *       Part Five:  Eventpool control api
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "events.h"


/*---------------------------------------------
 *             Part One: Define
-*---------------------------------------------*/

#define PER_PEV_SIZE    sizeof(Epollev)

#define IS_INVAILD_POOL(pool) \
    do { \
        if (!pool) { \
            errno = EINVAL; \
            return  false; \
        } \
    } while (0)

#define PROC_EVENT(ev, has_event, proc) \
    if (has_event && ev->proc) \
        ev->proc(ev->fd);


/*---------------------------------------------
 *          Part Three: Local function
-*---------------------------------------------*/


/*---------------------------------------------
 *    Part Four: Eventpool open close api
 *
 *          1. events_create
 *          2. events_destroy
 *
-*---------------------------------------------*/

/*-----events_create-----*/
bool events_create(Eventpool *pool, uint32_t max_proc)
{
    if (!pool || max_proc < 1) {
        errno = EINVAL;
        return  false;
    }

    if ((pool->ep_fd = epoll_create1(EPOLL_CLOEXEC)) == -1)
        return  false;

    pool->ev_maxproc = max_proc; 
    pool->ev_cnt = 0;

    return  true;
}


/*-----events_destroy-----*/
bool events_destroy(Eventpool *pool)
{
    IS_INVAILD_POOL(pool);

    if (close(pool->ep_fd) == -1)
        return  false;

    pool->ev_cnt = pool->ev_maxproc = 0;
    pool->ep_fd = 0;

    return  true;
}


/*---------------------------------------------
 *     Part Five: Eventpool control api
 *
 *          1. events_run 
 *
-*---------------------------------------------*/

/*-----events_runforever-----*/
bool events_run(Eventpool *pool, int32_t times, int32_t timeout)
{
    IS_INVAILD_POOL(pool);

    int32_t     nevents, cnt = 0;
    Epollev    *eventlist = alloca(pool->ev_maxproc * PER_PEV_SIZE);

    if (!eventlist)
        return  false;

    while (times == INF_TIMES || cnt++ < times) {
        nevents = epoll_wait(pool->ep_fd, eventlist, pool->ev_maxproc, timeout);

        if (nevents == -1)
            return  false;

        for (int32_t idx = 0; idx < nevents; idx++) {
            Event   *ev = (Event *)eventlist[idx].data.ptr;
            int32_t  epev = eventlist[idx].events; 

            PROC_EVENT(ev, epev & EPOLLIN, reader);
            PROC_EVENT(ev, epev & EPOLLOUT, writer);
            PROC_EVENT(ev, epev & EPOLLHUP, hanger);
            PROC_EVENT(ev, epev & EPOLLERR, errer);
        }
    }

    return  true;
}

