/*---------------------------------------------
 *     modification time: 2016.11.23 15:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: events.c
 *     creation time: 2016.11.03 15:05
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
 *       Part Four:  Events control
 *       Part Five:  Events operate
 *       Part Six:   Events helper
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "events.h"


/*---------------------------------------------
 *              Part One: Define
-*---------------------------------------------*/

#define IS_INVALID_EVENT(events) \
    if (!events) { \
        errno = EINVAL; \
        return  false; \
    }


#define EVENT_ACTION(triggered, type) \
    if (triggered) { \
        Event  *curr = eventsaver_search(saver, type, fd); \
\
        if (curr) \
            curr->data.handle(fd, curr->data.args); \
    }


/*---------------------------------------------
 *           Part Two: Local data 
-*---------------------------------------------*/

static Evdata   DefaultEvdata = {NULL, NULL};


/*---------------------------------------------
 *         Part Three: Local function 
-*---------------------------------------------*/

static bool _events_ctl(Events *events, int32_t fd,
            int32_t op, int32_t type, Evdata *data);


/*---------------------------------------------
 *          Part Four: Events control
 *
 *             1. events_create
 *             2. events_destroy
 *             3. events_run
 *
-*---------------------------------------------*/

/*-----events_create-----*/
bool events_create(Events *events, uint32_t max_proc)
{
    if (!events || max_proc < 1) {
        errno = EINVAL;
        return  false;
    }

    if ((events->ep_fd = epoll_create1(EPOLL_CLOEXEC)) == -1)
        return  false;

    if (!eventsaver_create(&events->ev_saver))
        return  false;

    events->ev_maxproc = max_proc;

    return  true; 
}


/*-----events_destroy-----*/
bool events_destroy(Events *events)
{
    IS_INVALID_EVENT(events);

    events->ev_maxproc = 0;

    if (close(events->ep_fd) == -1) {
        if (errno == EBADF)
            return  false;
    }

    events->ep_fd = 0;

    if (!eventsaver_destroy(&events->ev_saver))
        return  false;

    return  true;
}


/*-----events_run-----*/
bool events_run(Events *events, int32_t times, int32_t timeout)
{
    IS_INVALID_EVENT(events)

    Epollev *evlist = alloca(events->ev_maxproc * sizeof(Epollev));

    if (!evlist)
        return  false;

    int32_t     nevent, cnt = 0;
    Eventsaver *saver = &events->ev_saver;

    while (times == INF_TIMES || cnt++ < times) {
        nevent = epoll_wait(events->ep_fd, evlist, events->ev_maxproc, timeout);

        if (nevent == -1)
            return  false;

        for (int idx = 0; idx < nevent; idx++) {
            Epollev *ev = evlist + idx;
            int32_t  fd = ev->data.fd;

            EVENT_ACTION(ev->events & EPOLLIN, EVREAD)
            EVENT_ACTION(ev->events & EPOLLOUT, EVWRITE)
            EVENT_ACTION(ev->events & EPOLLERR, EVERROR)
        }
    }

    return  true;
}


/*---------------------------------------------
 *         Part Five: Events operate
 *
 *             1. events_ctl
 *
-*---------------------------------------------*/

/*-----events_ctl-----*/
bool events_ctl(Events *events, int32_t fd, 
        int32_t op, int32_t type, uint32_t event, Evdata *data)
{
    IS_INVALID_EVENT(events);

    Epollev ev;

    ev.events = event;
    ev.data.fd = fd;

    if (epoll_ctl(events->ep_fd, op, fd, &ev) == -1)
        return  false;

    if (!_events_ctl(events, fd, op, type, data))
        return  false;

    return  true;
}


/*---------------------------------------------
 *         Part Six: Events helper 
 *
 *             1. _events_ctl 
 *
-*---------------------------------------------*/

/*-----_events_ctl-----*/
bool _events_ctl(Events *events, 
        int32_t fd, int32_t op, int32_t type, Evdata *data)
{
    if (!data)
        data = &DefaultEvdata;

    switch (type) {
        case EPOLL_CTL_ADD:
        case EPOLL_CTL_MOD:
        case EPOLL_CTL_DEL:

        default:
            errno = EINVAL;
            return  false;
    }

    return  true;
}

