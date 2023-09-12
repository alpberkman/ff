
#include <stdio.h>


#define CELL_TYPE unsigned short
#define MEM_SIZE (0x8000)
//#define PS_SIZE (0x100)
//#define RS_SIZE (0x100)


#define FALSE (0)
#define TRUE (-1)

#define MASK_VIS (1<<7)
#define MASK_IMM (1<<6)
#define WORD_LEN (31)


#define CELL_SIZE ((cell) sizeof(cell))


typedef CELL_TYPE cell;
typedef unsigned char byte;

typedef enum power power;
typedef enum state state;
typedef struct VM VM;
typedef enum prim prim;


void _nop(VM *vm);
void _lit(VM *vm);
void _halt(VM *vm);
void _dup(VM *vm);
void _drop(VM *vm);
void _swap(VM *vm);
void _push(VM *vm);
void _pop(VM *vm);
void _pick(VM *vm);
void _rick(VM *vm);
void _jmp(VM *vm);
void _jz(VM *vm);
void _ret(VM *vm);
void _eq(VM *vm);
void _neq(VM *vm);
void _gt(VM *vm);
void _lt(VM *vm);
void _and(VM *vm);
void _or(VM *vm);
void _xor(VM *vm);
void _shr(VM *vm);
void _shl(VM *vm);
void _tru(VM *vm);
void _fls(VM *vm);
void _add(VM *vm);
void _sub(VM *vm);
void _mul(VM *vm);
void _div(VM *vm);
void _mod(VM *vm);
void _ldc(VM *vm);
void _strc(VM *vm);
void _ldb(VM *vm);
void _strb(VM *vm);
void _cell(VM *vm);
void _mem(VM *vm);
void _ldp(VM *vm);
void _strp(VM *vm);
void _ldr(VM *vm);
void _strr(VM *vm);
void _ldi(VM *vm);
void _stri(VM *vm);
void _key(VM *vm);
void _emit(VM *vm);
void _call(VM *vm);


cell next(VM *vm);
void exec(VM *vm, cell opcode);
void tick(VM *vm);
void run(VM *vm);

void dump(VM *vm, char *rom);
void load(VM *vm, char *rom);
void carr(VM *vm, char *rom);


#ifndef FF_IMPL
#define FF_IMPL

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
    //state s;

    cell ps[0x100];
    byte psp;

    cell rs[0x100];
    byte rsp;

    byte mem[MEM_SIZE];
    cell ip;
    //cell hp;
    //cell lp;
};

enum prim {
NOP, LIT, HALT,
// Stack
DUP, DROP, SWAP,
PUSH, POP,
PICK, RICK,
// Controlflow
JMP, JZ, RET,
// Logic
EQ, NEQ, GT, LT,
AND, OR, XOR,
SHR, SHL,
TRU, FLS,
// Arithmetic
ADD, SUB, MUL, DIV, MOD,
// Memory
LDC, STRC,
LDB, STRB,
CELL, MEM,
// Special
LDP, STRP,
LDR, STRR,
LDI, STRI,
// IO
KEY, EMIT,
// Call
CALL,
};


void _nop(VM *vm) {
    (void) vm;
}
void _lit(VM *vm) {
    cell val = *((cell *) &(vm->mem[vm->ip]));
    vm->ps[vm->psp++] = val;
    vm->ip += CELL_SIZE;
}
void _halt(VM *vm) {
    vm->p = OFF;
}
void _dup(VM *vm) {
    cell val = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = val;
    vm->ps[vm->psp++] = val;
}
void _drop(VM *vm) {
    --vm->psp;
}
void _swap(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = a;
    vm->ps[vm->psp++] = b;
}
void _push(VM *vm) {
    vm->rs[vm->rsp++] = vm->ps[--vm->psp];
}
void _pop(VM *vm) {
    vm->ps[vm->psp++] = vm->rs[--vm->rsp];
}
void _pick(VM *vm) {
    byte n = vm->ps[--vm->psp] + 1;
    n = vm->psp - n;
    vm->ps[vm->psp++] = vm->ps[n];
}
void _rick(VM *vm) {
    byte n = vm->ps[--vm->psp] + 1;
    n = vm->rsp - n;
    vm->ps[vm->psp++] = vm->rs[n];
}
void _jmp(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    vm->ip = addr;
}
void _jz(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    cell flag = vm->ps[--vm->psp];
    if(flag == ((cell) 0))
        vm->ip = addr;
}
void _ret(VM *vm) {
    cell addr = vm->rs[--vm->rsp];
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
void _gt(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b > a ? TRUE : FALSE;
}
void _lt(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b < a ? TRUE : FALSE;
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
void _tru(VM *vm) {
    vm->ps[vm->psp++] = TRUE;
}
void _fls(VM *vm) {
    vm->ps[vm->psp++] = FALSE;
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
    cell addr = vm->ps[--vm->psp];
    cell val = *((cell *) &(vm->mem[addr]));
    vm->ps[vm->psp++] = val;
}
void _strc(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    cell val = vm->ps[--vm->psp];
    *((cell *) &(vm->mem[addr])) = val;
}
void _ldb(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    byte val = vm->mem[addr];
    vm->ps[vm->psp++] = val;
}
void _strb(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    byte val = vm->ps[--vm->psp];
    vm->mem[addr] = val;
}
void _cell(VM *vm) {
    vm->ps[vm->psp++] = CELL_SIZE;
}
void _mem(VM *vm) {
    vm->ps[vm->psp++] = MEM_SIZE;
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
    cell val = vm->ip;
    vm->ps[vm->psp++] = val;
}
void _stri(VM *vm) {
    cell val = vm->ps[--vm->psp];
    vm->ip = val;
}
void _key(VM *vm) {
    vm->ps[vm->psp++] = getchar();
}
void _emit(VM *vm) {
    putchar(vm->ps[--vm->psp]);
}
void _call(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    vm->ip = addr;
}

cell next(VM *vm) {
    cell opcode = *((cell *) &(vm->mem[vm->ip]));
    vm->ip += CELL_SIZE;
    return opcode;
}
void exec(VM *vm, cell opcode) {
    switch(opcode) {
        case NOP: _nop(vm); break;
        case LIT: _lit(vm); break;
        case HALT: _halt(vm); break;
        case DUP: _dup(vm); break;
        case DROP: _drop(vm); break;
        case SWAP: _swap(vm); break;
        case PUSH: _push(vm); break;
        case POP: _pop(vm); break;
        case PICK: _pick(vm); break;
        case RICK: _rick(vm); break;
        case JMP: _jmp(vm); break;
        case JZ: _jz(vm); break;
        case RET: _ret(vm); break;
        case EQ: _eq(vm); break;
        case NEQ: _neq(vm); break;
        case GT: _gt(vm); break;
        case LT: _lt(vm); break;
        case AND: _and(vm); break;
        case OR: _or(vm); break;
        case XOR: _xor(vm); break;
        case SHR: _shr(vm); break;
        case SHL: _shl(vm); break;
        case TRU: _tru(vm); break;
        case FLS: _fls(vm); break;
        case ADD: _add(vm); break;
        case SUB: _sub(vm); break;
        case MUL: _mul(vm); break;
        case DIV: _div(vm); break;
        case MOD: _mod(vm); break;
        case LDC: _ldc(vm); break;
        case STRC: _strc(vm); break;
        case LDB: _ldb(vm); break;
        case STRB: _strb(vm); break;
        case CELL: _cell(vm); break;
        case MEM: _mem(vm); break;
        case LDP: _ldp(vm); break;
        case STRP: _strp(vm); break;
        case LDR: _ldr(vm); break;
        case STRR: _strr(vm); break;
        case LDI: _ldi(vm); break;
        case STRI: _stri(vm); break;
        case KEY: _key(vm); break;
        case EMIT: _emit(vm); break;
        case CALL: _call(vm); break;
        default:
            vm->rs[vm->rsp++] = vm->ip;
            vm->ip = opcode;
            break;
    }
}
void tick(VM *vm) {
    exec(vm, next(vm));
}
void run(VM *vm) {
    vm->p = ON;
    vm->psp = 0;
    vm->rsp = 0;
    while(vm->p == ON)
        tick(vm);
}

void dump(VM *vm, char *rom) {
    FILE *fptr = fopen(rom, "w");
    if(fptr == NULL) {
        printf("[DUMP]: Can't open file\n");
        return;
    }

    for(int i = 0; i < MEM_SIZE; ++i)
        fputc(vm->mem[i], fptr);
}
void load(VM *vm, char *rom) {
    FILE *fptr = fopen(rom, "r");
    if(fptr == NULL) {
        printf("[LOAD]: Can't open file\n");
        return;
    }

    for(int i = 0; i < MEM_SIZE; ++i)
        vm->mem[i] = fgetc(fptr);
}
void carr(VM *vm, char *rom) {
    FILE *fptr = fopen(rom, "w");
    if(fptr == NULL) {
        printf("[CARR]: Can't open file\n");
        return;
    }

    fprintf(fptr, "\n\n{");
    for(int i = 0; i < MEM_SIZE; ++i) {
        if(i%8 == 0) fprintf(fptr, "\n\t");
        fprintf(fptr, "%03x, ", vm->mem[i]);
    }
    fprintf(fptr, "\n};\n\n");
}

#endif

















