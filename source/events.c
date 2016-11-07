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
 *       Source file content Eight part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Eventpool control api
 *       Part Five:  Event control api
 *       Part Six:   Eventpool operate
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

#define PROC_EVENT(ev, has_event, proc) \
    if (has_event && ev->proc) \
        ev->proc(ev->fd);


/*---------------------------------------------
 *          Part Three: Local function
-*---------------------------------------------*/

static bool _event_delete(Eventpool *pool, int32_t fd);
static bool _event_cmper(const void *er, const void *fd);


/*---------------------------------------------
 *    Part Four: Eventpool control api
 *
 *          1. events_create
 *          2. events_destroy
 *          3. events_run
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

    int32_t res;

    if ((res = pthread_mutex_init(&pool->ev_poollock, NULL))) {
        errno = res;
        return  false;
    }

    if (!mmdp_create(&pool->ev_mempool, DEF_CHUNKSIZE))
        return  false;

    if (!list_init(&pool->ev_list, _event_cmper))
        return  false;

    pool->ev_maxproc = max_proc; 
    pool->ev_cnt = 0;

    return  true;
}


/*-----events_destroy-----*/
bool events_destroy(Eventpool *pool)
{
    IS_INVAILD_POOL(pool);

    pool->ev_cnt = pool->ev_maxproc = 0;

    if (close(pool->ep_fd) == -1)
        return  false;

    pool->ep_fd = 0;

    int32_t res;

    if ((res = pthread_mutex_destroy(&pool->ev_poollock))) {
        errno = res;
        return  false;
    }
     
    mmdp_free_pool(&pool->ev_mempool);

    return  true;
}


/*-----events_run-----*/
bool events_run(Eventpool *pool, int32_t times, int32_t timeout)
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
            PROC_EVENT(ev, epev & EPOLLHUP, hanger);
            PROC_EVENT(ev, epev & EPOLLERR, errer);

            if (ev->need_del && !_event_delete(pool, ev->fd))
                return  false;
        }
    }

    UNLOCK_POOL(pool);

    return  true;
}


/*---------------------------------------------
 *       Part Five: Event control api
 *
 *          1. event_set_delete
 *          2. event_delete
 *
-*---------------------------------------------*/

/*-----event_set_delete-----*/
bool event_set_delete(Eventpool *pool, int32_t fd)
{
    Event  *ev = list_search(&pool->ev_list, &fd);

    if (ev) {
        ev->need_del = true;
        return  true;
    }

    errno = ENOENT; 
    return  false;
}


/*-----event_delete-----*/
bool event_delete(Eventpool *pool, int32_t fd)
{
    LOCK_POOL(pool);

    bool    result = true;

    if (!_event_delete(pool, fd))
        result = false;

    UNLOCK_POOL(pool);

    return  result;
}


/*---------------------------------------------
 *     Part Six: Eventpool operate
 *
 *          1. _event_delete
 *          2. _event_cmper
 *
-*---------------------------------------------*/

/*-----_event_delete-----*/
bool _event_delete(Eventpool *pool, int32_t fd)
{
    Epollev useless;

    if (epoll_ctl(pool->ep_fd, EPOLL_CTL_DEL, fd, &useless) == -1)
        return  false;

    close(fd);

    Event  *ev = list_delete(&pool->ev_list, &fd);

    if (ev) {
        mmdp_free(&pool->ev_mempool, ev);
        return  true;
    }

    return  false;
}


/*-----_event_cmper-----*/
bool _event_cmper(const void *er, const void *fd)
{
    return  ((Event *)er)->fd == *((int32_t *)fd);
}


