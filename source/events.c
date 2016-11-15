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

#define EVENT_ACTION(events, triggered, epfd, fd, type) \
    if (triggered && (events)->ev_func) \
        (events)->ev_func(events->ev_obj, fd, type);


/*---------------------------------------------
 *    Part Four: Events control api
 *
 *          1. events_create
 *          2. events_destroy
 *          3. events_run
 *
-*---------------------------------------------*/

/*-----events_create-----*/
int32_t events_create(Events *events, uint32_t max_proc,
            void *evobj, ev_functor functor)
{
    if (!events || max_proc < 1 || !functor) {
        errno = EINVAL;
        return  -1;
    }

    if ((events->ep_fd = epoll_create1(EPOLL_CLOEXEC)) == -1)
        return  -1;

    events->ev_maxproc = max_proc;
    events->ev_func = functor;
    events->ev_obj = evobj;

    return  events->ep_fd;
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
    events->ev_func = NULL;
    events->ev_obj = NULL;

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

    int32_t  nevent, cnt = 0;
    int32_t  epfd = events->ep_fd, maxproc = events->ev_maxproc;

    while (times == INF_TIMES || cnt++ < times) {
        nevent = epoll_wait(epfd, evlist, maxproc, timeout);

        if (nevent == -1)
            return  false;

        for (int idx = 0; idx < nevent; idx++) {
            Epollev *ev = evlist + idx;

            EVENT_ACTION(events,
                ev->events & EPOLLIN, epfd, ev->data.fd, EVREAD)

            EVENT_ACTION(events,
                ev->events & EPOLLOUT, epfd, ev->data.fd, EVWRITE)

            EVENT_ACTION(events,
                ev->events & EPOLLERR, epfd, ev->data.fd, EVERROR)
        }
    }

    return  true;
}


