/*---------------------------------------------
 *     modification time: 2016-11-23 22:30:21
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: example.c
 *     creation time: 2016-07-12 18:29:21
 *     author: Muse
-*---------------------------------------------*/

#include "Events.h"


void timer_get(int32_t fd, void *args)
{
    uint64_t    value;
    Events     *events = (Events *)args;

    read(fd, &value, sizeof(uint64_t));
    printf("%lu\n", value);

    events_stop_run(events);
}


void signal_get(int32_t fd, void *args)
{
    struct signalfd_siginfo sinfo;
    Events *events = (Events *)args;

    read(fd, &sinfo, sizeof(struct signalfd_siginfo));

    printf("Fd: %d\n", sinfo.ssi_signo);
    printf("Code: %d\n", sinfo.ssi_code);
    printf("Pid: %d\n", sinfo.ssi_pid);

    if (!events_ctl(events, fd,
            EPOLL_CTL_DEL, EVREAD, 0, NULL))
        perror("events_ctl");

    Itimerspec  value;
    Evdata      data;

    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;
    value.it_value.tv_sec = 2;
    value.it_value.tv_nsec = 0;

    data.handle = timer_get;
    data.args = events;

    if (timerfd_add(events, CLOCK_REALTIME, &value, &data) == -1)
        perror("timerfd_add");
}


int main(void)
{
    Events  events;

    if (!events_create(&events, 16)) {
        perror("events_create");
        return  -1;
    }

    Evdata  data;

    data.handle = signal_get;
    data.args = &events;

    if (signalfd_add(&events, SIGINT, &data) == -1) {
        perror("signalfd_add");
        return  -1;
    }

    if (!events_run(&events, INF_TIMES, -1))
        perror("events_run");

    if (!events_destroy(&events)) {
        perror("events_destroy");
        return  -1;
    }

    return  1;
}

