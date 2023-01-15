
CC ?= gcc
CFLAGS ?= -Wall -O2
DFLAGS ?= -g
SFLAGS ?= -Os -s

.PHONY: ff

all: ff

ff: main.c ff.h
	$(CC) $(CFLAGS) $(SFLAGS) -o $@ $^

clean:
	rm -rf *.o *.optimized *.out debug ff

