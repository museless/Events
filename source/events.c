/*---------------------------------------------
 *     modification time: 2016.11.07 10:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: events.c
 *     creation time: 2016.11.03 15:05
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
 *       Part Four:  Events control api
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

#define LOCK_POOL(pool) \
    do { \
        int32_t result; \
        if ((result = pthread_mutex_trylock(&pool->ev_poollock))) { \
            errno = result; \
            return  false; \
        } \
    } while (0) \

#define UNLOCK_POOL(pool) \
    pthread_mutex_unlock(&pool->ev_poollock);


/*---------------------------------------------
 *          Part Three: Local data
-*---------------------------------------------*/


/*---------------------------------------------
 *          Part Three: Local function
-*---------------------------------------------*/

static bool _event_delete(Events *pool, int32_t fd);
static bool _event_cmper(const void *er, const void *fd);


/*---------------------------------------------
 *    Part Four: Events control api
 *
 *          1. events_create
 *          2. events_destroy
 *          3. events_run
 *
-*---------------------------------------------*/

/*-----events_create-----*/
bool events_create(Events *pool, uint32_t max_proc)
{
    if (!pool || max_proc < 1) {
        errno = EINVAL;
        return  false;
    }

    if ((pool->ep_fd = epoll_create1(EPOLL_CLOEXEC)) == -1)
        return  false;

    int32_t res;

    if ((res = pthread_mutex_init(&pool->ev_poollock, NULL))) {
        errno = res;
        return  false;
    }

    pool->ev_maxproc = max_proc; 

    return  true;
}


/*-----events_destroy-----*/
bool events_destroy(Events *pool)
{
    IS_INVAILD_POOL(pool);
    LOCK_POOL(pool);

    pool->ev_maxproc = 0;

    if (close(pool->ep_fd) == -1) {
        if (errno == EBADF)
            return  false;
    }

    pool->ep_fd = 0;

    int32_t res;

    if ((res = pthread_mutex_destroy(&pool->ev_poollock))) {
        errno = res;
        return  false;
    }

    return  true;
}


/*-----events_run-----*/
bool events_run(Events *pool, int32_t times, int32_t timeout)
{
    IS_INVAILD_POOL(pool);
    LOCK_POOL(pool);

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
            PROC_EVENT(ev, epev & EPOLLERR, errer);

            if (ev->need_del && !_event_delete(pool, ev->fd))
                return  false;
        }
    }

    UNLOCK_POOL(pool);

    return  true;
}


