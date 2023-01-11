
CC ?= gcc
CFLAGS ?= -g -Wall -O2
DFLAGS ?= -DDEBUG_REPL -DDEBUG_MEM

.PHONY: run

all: ff 
	clear
	./ff
ff: main.c  opcodes.c  repl.c  utils.c  vm.c
	$(CC) $(CFLAGS) -o $@ $^

run: ff
	./ff

debug: main.c  opcodes.c  repl.c  utils.c  vm.c
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ $^


clean:
	rm -rf *.o *.optimized *.out debug ff


