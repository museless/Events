# flags #
CC		= gcc
CFLAGS	= -g -Wall -std=c99 -D_DEFAULT_SOURCE

# elf #
EXMDP	= test

# obj #
OMDP	= source/events.c source/events.c 

# phony #
.phony:	build

build	: $(EXMDP)

$(EXMDP) : $(OMDP)
	$(CC) -o $(EXMDP) $(OMDP) $(CFLAGS)