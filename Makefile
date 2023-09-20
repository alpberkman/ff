
CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2 -Wno-unused-variable
DFLAGS ?= -g -ansi
SFLAGS ?= -Os -s

.PHONY: ff

all: ff

ff: main.c ff.c ff_init.c ff_debug.c ff_interp.c
	$(CC) $(CFLAGS) $(SFLAGS) -o $@ $^

clean:
	rm -rf *.o *.optimized *.out debug ff *.orig

