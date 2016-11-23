/*---------------------------------------------
 *     modification time: 2016.11.08 22:00
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
 *         Part Three: Local function 
-*---------------------------------------------*/

#define EVENT_ACTION(triggered, fd, type) \
    if (triggered) { \
        Event  *curr = eventsaver_search(saver, type, fd); \
\
        if (curr) \
            curr->handle(fd, curr->args); \
    }


/*---------------------------------------------
 *    Part Four: Events control api
 *
 *          1. events_create
 *          2. events_destroy
 *          3. events_run
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
    if (!events) {
        errno = EINVAL;
        return  false;
    }

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
    if (!events) {
        errno = EINVAL;
        return  false;
    }

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

            EVENT_ACTION(ev->events & EPOLLIN, ev->data.fd, EVREAD)
            EVENT_ACTION(ev->events & EPOLLOUT, ev->data.fd, EVWRITE)
            EVENT_ACTION(ev->events & EPOLLERR, ev->data.fd, EVERROR)
        }
    }

    return  true;
}


