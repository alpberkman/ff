#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define MEM_SIZE (0x8000)


#define FALSE 0
#define TRUE -1

#define MASK_VIS (1<<7)
#define MASK_IMM (1<<6)
#define WORD_LEN (0x1f)

#define CELL_SIZE ((cell) sizeof(cell))


typedef signed short cell;
typedef unsigned char byte;

typedef enum power power;
typedef enum state state;
typedef struct VM VM;
typedef enum prim prim;


enum power {
    OFF = FALSE,
    ON = TRUE,
};

enum state {
    INTERPRET = FALSE,
    COMPILE = TRUE,
};

struct VM {
    power p;
    state s;

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
    LIT, CSZ, HALT,

    DUP, DROP, SWAP, PUSH, POP,
    PICKP, PICKR,

    JMP, JZ, CALL, RET,

    EQ, NEQ, LT, GT,
    AND, OR, XOR, SHR, SHL,
    ADD, SUB, MUL, DIV, MOD,

    LDC, STRC,
    LDB, STRB,

    LDS, STRS,
    LDP, LDR

    LDH, STRH,
    LDL, STRL,

    KEY, EMIT,

    SW, /*scan word*/
    FW, /*find word*/
    MH, /*make header*/
    TN, /*number*/
};













