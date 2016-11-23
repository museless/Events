/*---------------------------------------------
 *     modification time: 2016-11-23 22:30:21
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: example.c
 *     creation time: 2016-07-12 18:29:21
 *     author: Muse
-*---------------------------------------------*/

#include "events.h"
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>


void timer_get(int32_t fd, void *args)
{
    struct itimerspec   value;

    timerfd_gettime(fd, &value);

    printf("%ld\n", value.it_value.tv_sec);
}


int32_t create_timer(void)
{
    int32_t tfd = timerfd_create(CLOCK_REALTIME, 0);

    if (tfd == -1)
        return  -1;

    struct itimerspec   value;

    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;
    value.it_value.tv_sec = 2;
    value.it_value.tv_nsec = 0;

    timerfd_settime(tfd, 0, &value, NULL);

    return  tfd;
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

    int32_t tfd = create_timer();

    if (tfd == -1) {
        perror("create_timer");
        return;
    }

    Evdata  data;

    data.handle = timer_get;
    data.args = events;

    if (!events_ctl(events, tfd,
            EPOLL_CTL_ADD, EVREAD, EPOLLIN | EPOLLET, &data))
        perror("events_ctl");
}


int32_t create_signalfd(void)
{
    sigset_t    set;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
        return  -1;

    return  signalfd(-1, &set, SFD_NONBLOCK);
}


int main(void)
{
    Events  events;

    if (!events_create(&events, 16)) {
        perror("events_create");
        return  -1;
    }

    int32_t sigfd = create_signalfd();

    if (sigfd == -1) {
        perror("create_signalfd");
        return  -1;
    }

    Evdata  data;

    data.handle = signal_get;
    data.args = &events;

    if (!events_ctl(&events, sigfd,
            EPOLL_CTL_ADD, EVREAD, EPOLLIN, &data)) {
        perror("events_ctl");
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

