/*---------------------------------------------
 *     modification time: 2016-11-23 22:30:21
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: example.c
 *     creation time: 2016-07-12 18:29:21
 *     author: Muse
-*---------------------------------------------*/

#include <events/eventss.h>


/* static */
static void come_connect(int32_t fd, int ev, void *args); 
static void writeback(int32_t fd, int ev, void *args);


/* define */
#define ADDR    "127.0.0.1"
#define PORT    8886

#define HTTPRET "HTTP/1.1 200 OK\r\n\r\nHello world"


int main(void)
{
    Events  events;

    if (!events_create(&events, 16)) {
        perror("events_create");
        return  -1;
    }

    Sockfd  sockfd;

    sockfd.addr.sin_family = AF_INET;
    sockfd.addr.sin_port = htons(PORT);
    sockfd.addr.sin_addr.s_addr = inet_addr(ADDR);

    sockfd.data.handle = come_connect;
    sockfd.data.args = &events;

    sockfd.ev = EPOLLIN;
    sockfd.backlog = 8;

    if (sockfd_bind(&events, &sockfd) == -1) {
        perror("sockfd_bind_add");
        return  -1;
    }

    if (!events_run(&events, INF_TIMES, INF_TIMES))
        perror("events_run");

    if (!events_destroy(&events)) {
        perror("events_destroy");
        return  -1;
    }

    return  1;
}


void come_connect(int32_t fd, int ev, void *params)
{
    Events *events = (Events *)params;
    int32_t user = accept(fd, NULL, NULL);

    if (user == -1)
        return;

    Evdata  data;

    data.handle = writeback;
    data.args = events;

    events_ctl(events, user, EPOLL_CTL_ADD, EPOLLOUT | EPOLLET, &data);
}


void writeback(int32_t fd, int ev, void *params)
{
    Events *events = (Events *)params;
    char    buff[1024];

    read(fd, buff, 1024);
    write(fd, HTTPRET, strlen(HTTPRET));

    eventfd_ctl(events, fd, NULL);
    close(fd);
}

