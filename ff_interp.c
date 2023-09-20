
#include "ff.h"
#include "ff_interp.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define streq(X, Y) (strcmp((X), (Y)) == 0)

extern cell hp;
extern cell lp;

extern cell hp_addr;
extern cell lp_addr;
extern cell st_addr;

byte buf[WORD_LEN+1];
cell len = 0;

void read() {
    int c;

    /*Remove white space*/
    while(isspace(c = getchar()));

    /*Get the string*/
    len = 0;
    do {
        if(c == EOF)
            return;
        buf[len++] = toupper(c);
    } while(len < WORD_LEN && !isspace(c = getchar()));
    buf[len] = '\0';
}


cell find(VM *vm) {
    cell addr;
    byte flags;

    for(addr = lp; addr != 0; addr = *((cell *) &(vm->mem[addr]))) {
        flags = vm->mem[addr + CELL_SIZE];
        if((flags & MASK_VIS) && len == (flags & WORD_LEN))
            if(strncmp((char *) buf, (char *) &(vm->mem[addr + CELL_SIZE + 1]), len) == 0)
                break;
    }

    if(addr == 0)
        return 0;
    else
        return addr + CELL_SIZE + 1 + len;
}


// postpone ' [']
void interp(VM *vm) {
    vm->ip = 0;
    cell addr = find(vm);
    if(addr != 0) {
        (*((cell *) &(vm->mem[0]))) = addr;
        (*((cell *) &(vm->mem[CELL_SIZE]))) = HALT;
    } else {
        (*((cell *) &(vm->mem[0]))) = LIT;
        (*((cell *) &(vm->mem[CELL_SIZE]))) = atoi((char *)buf);
        (*((cell *) &(vm->mem[CELL_SIZE*2]))) = HALT;
    }
}

void compile(VM *vm) {

}

void eval(VM *vm) {
    if(streq((char *)buf, "dump")) {
        dump(vm, "ff.dump");
        carr(vm, "rom.h");
    } else if((*((cell *) &(vm->mem[hp]))) == FFALSE)
        interp(vm);
    else
        compile(vm);
}



