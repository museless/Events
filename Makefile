# flags
CC		= gcc
CFLAGS  = -g -Wall -std=c99 -D_DEFAULT_SOURCE -pthread
CINC	= -I source/
CCOM	= -fPIC -c
CLIB	= -shared
CILIB	= -Wl,-rpath /usr/local/lib/ -L/usr/local/lib/ -lepollevents

# elf #
EXEV	= test
EXLIB	= /usr/local/lib/libepollevents.so
INCLDIC	= /usr/include/events

# obj #
OEXAM	= example.c 

EXOBJ	= source/events.o \
		  source/eventsaver.o source/mempool.o \
		  source/signalfd.o source/timerfd.o source/sockfd.o

# phony
.phony:	build buildinc lib clean cleanobj

# exec
buildinc:
	mkdir -p $(INCLDIC)
	cp source/*.h $(INCLDIC)

lib		: $(EXLIB) buildinc
build	: lib $(EXEV)
clean	:
	rm -rf test $(EXOBJ) $(EXLIB) $(INCLDIC)

cleanobj:
	rm -rf test $(EXOBJ)

# make
$(EXEV) : $(OEXAM)
	$(CC) -o $(EXEV) $(CILIB) $(CFLAGS) $(CINC) $(OEXAM)

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

