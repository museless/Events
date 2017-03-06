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
#include <arpa/inet.h>


/* static */
static void write_header(int32_t fd, int ev, void *args); 
static void read_html(int32_t fd, int ev, void *args);


/* define */
#define GET \
    "GET / HTTP/1.1\r\n" \
    "Host: money.163.com\r\n" \
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/* ;q=0.8\r\n" \
    "User-Agent: Lynx/2.8.7pre.5 libwww-FM/2.14 SSL-MM/1.4.1\r\n" \
    "Accept-Language: zh-CN\r\n\r\n"


int main(void)
{
    Events  events;

    if (!events_create(&events, 16)) {
        perror("events_create");
        return  -1;
    }

    Evdata      data;

    data.handle = write_header;
    data.args = &events;

    struct sockaddr_in  addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_addr("183.6.245.191");

    if (sockfd_connect_add(&events, EPOLLOUT, (Sockaddr *)&addr, &data) == -1) {
        perror("sockfd_connect_add");
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


void write_header(int32_t fd, int ev, void *params)
{
    Events *events = (Events *)params;

    if (write(fd, GET, strlen(GET)) == -1) {
        perror("write");
        return;
    }

    Evdata  data;

    data.handle = read_html;
    data.args = events;

    if (!eventfd_ctl(events, fd, &data))
        perror("eventfd_ctl");
}


void read_html(int32_t fd, int ev, void *params)
{
    Events *events = (Events *)params;
    char    buffer[BUFSIZ] = {0};

    read(fd, buffer, BUFSIZ - 1);

    printf("%s\n", buffer);

    read(fd, buffer, BUFSIZ - 1);
    printf("%s\n", buffer);

    events_stop_run(events);
}

