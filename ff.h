#include <stdio.h>

#define TRUE -1
#define FALSE 0


typedef struct VM VM;
typedef enum power power;
typedef enum state state;
typedef void (*fun)(VM *vm);

typedef signed short cell;
typedef unsigned short ptr;
typedef unsigned char byte;

typedef enum op op;


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

    cell ps[0x100];
    byte psp;

    cell rs[0x100];
    byte rsp;

    byte mem[0x10000];
    ptr ip;
    ptr hp;
    ptr lp;
};

enum op {
    NOP, LIT, HALT,

    DUP, DROP, SWAP,
    PUSH, POP,
    PICKP, PICKR,

    JMP, JZ,

    CALL, RET,

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

    KEY, EMIT,
};


void _nop(VM *vm) {
}

void _lit(VM *vm) {
    cell val = *((cell *) &(vm->mem[vm->ip]));
    vm->ps[vm->psp++] = val;
    vm->ip += sizeof(cell);
}
void _halt(VM *vm) {
    vm->p = OFF;
}

void _dup(VM *vm) {
    cell val = vm->ps[(byte) (vm->psp - 1)];
    vm->ps[vm->psp++] = val;
}

void _drop(VM *vm) {
    vm->psp -= 1;
}

void _swap(VM *vm) {
    cell tmp1 = vm->ps[--vm->psp];
    cell tmp2 = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = tmp1;
    vm->ps[vm->psp++] = tmp2;
}

void _push(VM *vm) {
    vm->rs[vm->rsp++] = vm->ps[--vm->psp];
}

void _pop(VM *vm) {
    vm->ps[vm->psp++] = vm->rs[--vm->rsp];
}

void _pickp(VM *vm) {
    byte n = vm->ps[--vm->psp] + 1;
    n = vm->psp - n;
    vm->ps[vm->psp++] = vm->ps[n];
}

void _pickr(VM *vm) {
    byte n = vm->ps[--vm->psp] + 1;
    n = vm->rsp - n;
    vm->ps[vm->psp++] = vm->rs[n];
}

void _jmp(VM *vm) {
    ptr addr = vm->ps[--vm->psp];
    vm->ip = addr;
}

void _jz(VM *vm) {
    ptr addr = vm->ps[--vm->psp];
    cell flag = vm->ps[--vm->psp];
    if(flag == 0)
        vm->ip = addr;
}

void _call(VM *vm) {
    ptr addr = vm->ps[--vm->psp];
    vm->rs[vm->rsp++] = vm->ip;
    vm->ip = addr;
}

void _ret(VM *vm) {
    ptr addr = vm->rs[--vm->rsp];
    vm->ip = addr;
}

void _eq(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b == a ? TRUE : FALSE;
}

void _neq(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b != a ? TRUE : FALSE;
}

void _lt(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b < a ? TRUE : FALSE;
}

void _gt(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b > a ? TRUE : FALSE;
}

void _and(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b & a;
}

void _or(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b | a;
}

void _xor(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b ^ a;
}

void _shr(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b >> a;
}

void _shl(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b << a;
}

void _add(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b + a;
}

void _sub(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b - a;
}

void _mul(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b * a;
}

void _div(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b / a;
}

void _mod(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b % a;
}

void _ldc(VM *vm) {
    ptr addr = vm->ps[--vm->psp];
    cell val = *((cell *) &(vm->mem[addr]));
    vm->ps[vm->psp++] = val;
}

void _strc(VM *vm) {
    ptr addr = vm->ps[--vm->psp];
    cell val = vm->ps[--vm->psp];
    *((cell *) &(vm->mem[addr])) = val;
}

void _ldb(VM *vm) {
    ptr addr = vm->ps[--vm->psp];
    byte val = vm->mem[addr];
    vm->ps[vm->psp++] = val;
}

void _strb(VM *vm) {
    ptr addr = vm->ps[--vm->psp];
    byte val = vm->ps[--vm->psp];
    vm->mem[addr] = val;
}

void _lds(VM *vm) {
    cell val = vm->s;
    vm->ps[vm->psp++] = val;
}

void _strs(VM *vm) {
    cell val = vm->ps[--vm->psp];
    vm->s = val;
}

void _ldp(VM *vm) {
    byte val = vm->psp;
    vm->ps[vm->psp++] = val;
}

void _strp(VM *vm) {
    byte val = vm->ps[--vm->psp];
    vm->psp = val;
}

void _ldr(VM *vm) {
    byte val = vm->rsp;
    vm->ps[vm->psp++] = val;
}

void _strr(VM *vm) {
    byte val = vm->ps[--vm->psp];
    vm->rsp = val;
}

void _ldi(VM *vm) {
    ptr val = vm->ip;
    vm->ps[vm->psp++] = val;
}

void _stri(VM *vm) {
    ptr val = vm->ps[--vm->psp];
    vm->ip = val;
}

void _ldh(VM *vm) {
    ptr val = vm->hp;
    vm->ps[vm->psp++] = val;
}

void _strh(VM *vm) {
    ptr val = vm->ps[--vm->psp];
    vm->hp = val;
}

void _ldl(VM *vm) {
    ptr val = vm->lp;
    vm->ps[vm->psp++] = val;
}

void _strl(VM *vm) {
    ptr val = vm->ps[--vm->psp];
    vm->lp = val;
}

void _key(VM *vm) {
    int c = getchar();
    vm->ps[vm->psp++] = c;
}

void _emit(VM *vm) {
    int c = vm->ps[--vm->psp];
    putchar(c);
}

fun prims[] = {
    _nop, _lit, _halt, _dup, _drop,
    _swap, _push, _pop,	_pickp, _pickr,
    _jmp, _jz, _call, _ret, _eq, _neq,
    _lt, _gt, _and, _or, _xor, _shr,
    _shl, _add, _sub, _mul, _div, _mod,
    _ldc, _strc, _ldb, _strb, _lds, _strs,
    _ldp, _strp, _ldr, _strr, _ldi, _stri,
    _ldh, _strh, _ldl, _strl, _key, _emit,
};

void init(VM *vm) {
    vm->p = ON;
    vm->s = INTERPRET;

    for(int i = 0; i < 0x100; ++i)
        vm->ps[i] = 0;
    vm->psp = 0;

    for(int i = 0; i < 0x100; ++i)
        vm->rs[i] = 0;
    vm->rsp = 0;

    for(int i = 0; i < 0x10000; ++i)
        vm->mem[i] = 0;
    vm->ip = 0;
    vm->hp = 0;
    vm->lp = 0;
}

byte opcode(VM *vm) {
    return vm->mem[vm->ip++];
}

void exec(VM *vm) {
    prims[opcode(vm)](vm);
}

void run(VM *vm) {
    while(vm->p == ON)
        exec(vm);
}

void debug(VM *vm) {
    printf("Debug Info\n");
    printf("Power: %s\n", vm->p == OFF ? "OFF" : "ON");
    printf("State: %s\n", vm->s == INTERPRET ? "INTERPRET" : "COMPILE");

    printf("PS: %6i\tRS: %6i\n", vm->psp, vm->rsp);
    for(int i = 0; i < (vm->psp > vm->rsp ? vm->psp : vm->rsp); ++i)
        printf("%10i\t%10i\n", vm->ps[i], vm->rs[i]);
/*
*/
    printf("IP: %i  HP: %i  LP: %i\n", vm->ip, vm->hp, vm->lp);
    for(int i = 0; i < vm->hp; ++i)
        printf("0x%04x: %3i\n", i, vm->mem[i]);

    printf("\n\n");
}






































