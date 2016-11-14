#flags #
CC		= gcc
CFLAGS	= -g -Wall -std=c99 -D_DEFAULT_SOURCE -pthread

# elf #
EXMDP	= test

# obj #
OMDP	= source/events.c source/example.c \
		  source/eventpool.c source/fdhash.c source/mempool.c

# phony #
.phony:	build

build	: $(EXMDP)

$(EXMDP) : $(OMDP)
	$(CC) -o $(EXMDP) $(OMDP) $(CFLAGS)
