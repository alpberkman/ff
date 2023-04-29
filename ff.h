#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define MEM_SIZE (0x8000)


#define TRUE -1
#define FALSE 0

#define MASK_VIS (1<<7)
#define MASK_IMM (1<<6)
#define WORD_LEN (0x1f)

#define CELL_SIZE ((cell) sizeof(cell))
#define CFUN_SIZE ((cell) sizeof(fun))


typedef struct VM VM;
typedef enum power power;
typedef enum state state;
typedef void (*fun)(VM *vm);

typedef signed short cell;
typedef unsigned char byte;

typedef enum prim prim;


enum power {
    OFF,
    ON,
};

enum state {
    INTERPRET,
    COMPILE,
};

struct VM {
    power p;
    state s;
    FILE *i;
    FILE *o;

    cell ps[0x100];
    byte psp;

    cell rs[0x100];
    byte rsp;

    byte mem[MEM_SIZE];
    cell ip;
    cell hp;
    cell lp;
};

enum prim {
    NOP = ((cell) MEM_SIZE),
    LIT, HALT,

    DUP, DROP, SWAP,
    PUSH, POP,
    PICKP, PICKR,

    JMP, JZ, RET,

    EQ, NEQ, LT, GT,

    AND, OR, XOR,
    SHR, SHL,

    ADD, SUB, MUL,
    DIV, MOD,

    LDC, STRC,
    LDB, STRB,

    LDS, STRS,

    LDP, STRP,
    LDR, STRR,

    LDI, STRI,
    LDH, STRH,
    LDL, STRL,

    CSZ, CFUN,
    KEY, EMIT, /*CALL,*/
};

/*
cell find_word(VM *vm, char *buf) {
    cell addr;
    byte flags;
    cell len;
    for(addr = vm->lp; addr != 0; addr = *((cell *) &(vm->mem[addr]))) {
        flags = vm->mem[addr + CELL_SIZE];
        len = strlen(buf);
        if((flags & MASK_VIS) && len == (flags & WORD_LEN))
            if(strncmp(buf, (char *) &(vm->mem[addr + CELL_SIZE + 1]), len) == 0)
                return addr;
    }
    return addr;
}
*/



#include "debug.h"


#include "prims.h"


#include "run.h"


#include "words.h"


#include "interp.h"








