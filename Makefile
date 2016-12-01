# flags
CC		= gcc
CFLAGS  = -g -Wall -std=c99 -D_DEFAULT_SOURCE -pthread
CINC	= -I source/
CCOM	= -fPIC -c
CLIB	= -shared
CILIB	= -L/Code/C/Events -levents

# elf #
EXEV	= test
EXLIB	= libevents.so


# obj #
OEXAM	= example.c 

EXOBJ	= source/events.o \
		  source/eventsaver.o source/mempool.o \
		  source/signalfd.o source/timerfd.o source/sockfd.o

# phony
.phony:	build object clean

# exec
object	: $(EXLIB)
build	: $(EXEV)
clean	:
	rm -f test $(EXOBJ) $(EXLIB)

$(EXEV) : $(OEXAM)
	$(CC) $(CFLAGS) $(CINC) $(CILIB) $(OEXAM) -o $(EXEV) 

$(EXLIB): $(EXOBJ)
	$(CC) $(CLIB) $(EXOBJ) -o $(EXLIB)


source/events.o: source/events.c
	$(CC) $(CFLAGS) $(CCOM) $(CINC) -o $@ $<

source/eventsaver.o: source/eventsaver.c
	$(CC) $(CFLAGS) $(CCOM) $(CINC) -o $@ $<

source/mempool.o: source/mempool.c
	$(CC) $(CFLAGS) $(CCOM) $(CINC) -o $@ $<

source/signalfd.o: source/signalfd.c
	$(CC) $(CFLAGS) $(CCOM) $(CINC) -o $@ $<

source/timerfd.o: source/timerfd.c
	$(CC) $(CFLAGS) $(CCOM) $(CINC) -o $@ $<

source/sockfd.o: source/sockfd.c
	$(CC) $(CFLAGS) $(CCOM) $(CINC) -o $@ $<

