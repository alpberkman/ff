
#include "ff.h"
#include "ff_interp.h"
#include "ff_init.h"
#include "ff_debug.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define streq(X, Y) (strcmp((char *)(X), (char *)(Y)) == 0)
#define update \
        (*((cell *) &(vm->mem[lp_addr]))) = lp; \
        (*((cell *) &(vm->mem[hp_addr]))) = hp;

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
        return addr;
}


// postpone ' [']
void interp(VM *vm) {
    if(streq(buf, "(")) {
        while(getchar() != ')');
    } else if(streq(buf, ":")) {
        (*((cell *) &(vm->mem[st_addr]))) = TTRUE;
        read();
        m_header(vm, (char *)buf, 0);
        m_word(vm, "[:]");
        *((cell *) &(vm->mem[hp])) =  hp - CELL_SIZE;
        hp += CELL_SIZE;
        update;
    } else if(streq(buf, "\'")) {
        read();
        cell addr = find(vm);
        (*((cell *) &(vm->mem[0]))) = LIT;
        (*((cell *) &(vm->mem[CELL_SIZE]))) = addr ? addr + CELL_SIZE + 1 + len : 0;
        (*((cell *) &(vm->mem[CELL_SIZE*2]))) = HALT;
    } else if(streq(buf, "VARIABLE")) {
        read();
        m_var(vm, (char *)buf);
        update;
    }
    /*else if(streq(buf, "CONSTANT")) {
        read();
        m_var(vm, (char *)buf);
        update;
    } */else {
        cell addr = find(vm);
        if(addr != 0) {
            (*((cell *) &(vm->mem[0]))) = addr + CELL_SIZE + 1 + len;
            (*((cell *) &(vm->mem[CELL_SIZE]))) = HALT;
        } else {
            (*((cell *) &(vm->mem[0]))) = LIT;
            (*((cell *) &(vm->mem[CELL_SIZE]))) = atoi((char *)buf);
            (*((cell *) &(vm->mem[CELL_SIZE*2]))) = HALT;
        }
    }
}

void compile(VM *vm) {
    if(streq(buf, "(")) {
        while(getchar() != ')');
    } else if(streq(buf, ";")) {
        m_word(vm, "[;]");
        vm->mem[lp + CELL_SIZE] |= MASK_VIS;
        (*((cell *) &(vm->mem[st_addr]))) = FFALSE;
        update;
    } else if(streq(buf, "[\']")) {
        read();
        cell addr = find(vm);
        m_number(vm, addr ? addr + CELL_SIZE + 1 + len : 0);
        update;
    } else if(streq(buf, "POSTPONE")) {
        read();
        m_word(vm, (char *)buf);
        update;
    } else {
        cell addr = find(vm);
        byte flags = vm->mem[addr + CELL_SIZE];
        if(addr != 0 && (flags & MASK_VIS)) {
            if(flags & MASK_IMM) {
                (*((cell *) &(vm->mem[0]))) = addr + CELL_SIZE + 1 + len;
                (*((cell *) &(vm->mem[CELL_SIZE]))) = HALT;
            } else {
                m_word(vm, (char *)buf);
                update;
            }
        } else {
            m_number(vm, atoi((char *)buf));
            update;
        }
    }
}

void eval(VM *vm) {
    vm->ip = 0;
    (*((cell *) &(vm->mem[0]))) = HALT;
    //printf("%s\n", buf);

    if(streq(buf, "DUMP")) {
        dump(vm, "ff.dump");
        carr(vm, "rom.h");
    } else if(streq(buf, "BYE")) {
        dump(vm, "dump.bin");
        carr(vm, "rom.h");
        exit(0);
    } else if(streq(buf, "L1")) {
        list1(vm);
    } else if(streq(buf, "L2")) {
        list2(vm);
    } else if((*((cell *) &(vm->mem[st_addr]))) == FFALSE) {
        interp(vm);
    } else {
        compile(vm);
    }
}


