
CC ?= gcc
CFLAGS ?= -Wall -O2
DFLAGS ?= -DDEBUG_REPL -DDEBUG_MEM -g
SFLAGS ?= -Os -s

.PHONY: run ff

all: ff

ff: main.c ff.h
	$(CC) $(CFLAGS) $(SFLAGS) -o $@ $^

run: ff
	clear
	./ff

debug: main.c ff.h
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ $^


clean:
	rm -rf *.o *.optimized *.out debug ff
