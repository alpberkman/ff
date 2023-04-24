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


typedef struct VM VM;
typedef enum power power;
typedef enum state state;
typedef void (*fun)(VM *vm);

typedef signed short cell;
typedef unsigned char byte;

typedef enum op op;

#define CELL_SIZE ((cell) sizeof(cell))
#define CFUN_SIZE ((cell) sizeof(fun))


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

enum op {
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

    LDH, STRH,
    LDL, STRL,

    CSZ, CFUN,
    KEY, EMIT, CALL,

    COL, SEMI, INTERP, DEB,
};


void _nop(VM *vm) {
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
    cell val = vm->ps[(byte) (vm->psp - 1)];
    vm->ps[vm->psp++] = val;
}

void _drop(VM *vm) {
    vm->psp -= 1;
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
    cell addr = vm->ps[--vm->psp];
    vm->ip = addr;
}

void _jz(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    cell flag = vm->ps[--vm->psp];
    if(flag == 0)
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

void _ldh(VM *vm) {
    cell val = vm->hp;
    vm->ps[vm->psp++] = val;
}

void _strh(VM *vm) {
    cell val = vm->ps[--vm->psp];
    vm->hp = val;
}

void _ldl(VM *vm) {
    cell val = vm->lp;
    vm->ps[vm->psp++] = val;
}

void _strl(VM *vm) {
    cell val = vm->ps[--vm->psp];
    vm->lp = val;
}

void _key(VM *vm) {
    int c = fgetc(vm->i);
    vm->ps[vm->psp++] = c;
}

void _emit(VM *vm) {
    int c = vm->ps[--vm->psp];
    fputc(c, vm->o);
}

void _csz(VM *vm) {
    vm->ps[vm->psp++] = CELL_SIZE;
}

void _cfun(VM *vm) {
	fun cfun = *((fun *) &(vm->mem[vm->ip]));
    vm->ip += CFUN_SIZE;
    cfun(vm);
}

/*
void _col(VM *vm) {

    char buf[32];
    int c;
    while(isspace(c = getchar()));

    byte len = 0;
    do {
        if(c == EOF)
            return;
        buf[len++] = c - (c >= 'a' && c <= 'z' ? 'a' - 'A' : 0);
    } while(len < 31 && !isspace(c = getchar()));
    buf[len] = '\0';

    vm->s = COMPILE;

    *((cell *) &(vm->mem[vm->hp])) = *((cell *) &(vm->lp));
    vm->lp = vm->hp;
    vm->hp += CELL_SIZE;

    byte i;
    for(i = 0; buf[i] != '\0'; ++i)
        vm->mem[vm->hp + 1 + i] = buf[i];

    vm->mem[vm->hp] = i;
    vm->hp += 1 + i;
}

void _semi(VM *vm) {
    vm->mem[vm->hp++] = RET;
    vm->mem[vm->lp + CELL_SIZE] |= MASK_VIS;
    vm->s = INTERPRET;
}

void _interp(VM *vm) {

    char buf[32];
    int c;
    while(isspace(c = getchar()));

    byte len = 0;
    do {
        if(c == EOF)
            return;
        buf[len++] = c - (c >= 'a' && c <= 'z' ? 'a' - 'A' : 0);
    } while(len < 31 && !isspace(c = getchar()));
    buf[len] = '\0';

    cell addr;
    byte flags;

    for(addr = vm->lp; addr != 0; addr = *((cell *) &(vm->mem[addr]))) {
        flags = vm->mem[addr + CELL_SIZE];
        if((flags & MASK_VIS) && len == (flags & WORD_LEN))
            if(strncmp(buf, (char *) &(vm->mem[addr + CELL_SIZE + 1]), len) == 0)
                break;
    }

    cell num = 0;
    byte nflag = 0;

    if(addr == 0) {
        nflag = strspn(buf, "1234567890-+") == len;
        if(nflag)
            num = atoi(buf);
    }

    if(vm->s == INTERPRET) {
        if(addr) {
            vm->rs[vm->rsp++] = vm->ip - 1;
            vm->ip = addr + CELL_SIZE + 1 + len;
        } else if(nflag) {
            vm->ps[vm->psp++] = num;
            vm->ip = vm->ip - 1;
        } else {
            vm->ip = vm->ip - 1;
            printf("Error in interpreter: word not found %s\n", buf);
        }
    } else if(vm->s == COMPILE) {
        if(addr) {
            if(flags & MASK_IMM) {
                vm->rs[vm->rsp++] = vm->ip - 1;
                vm->ip = addr + CELL_SIZE + 1 + len;
            } else {
                vm->mem[vm->hp++] = LIT;
                *((cell *) &(vm->mem[vm->hp])) = addr + CELL_SIZE + 1 + len;
                vm->hp += CELL_SIZE;
                vm->mem[vm->hp++] = CALL;
                vm->ip = vm->ip - 1;
            }
        } else if(nflag) {
            vm->mem[vm->hp++] = LIT;
            *((cell *) &(vm->mem[vm->hp])) = num;
            vm->hp += CELL_SIZE;
            vm->ip = vm->ip - 1;
        } else {
            vm->s = INTERPRET;
            vm->hp = vm->lp;
            vm->lp = *((cell *) &(vm->mem[vm->lp]));
            vm->ip = vm->ip - 1;
            vm->psp = 0;
            vm->rsp = 0;
            printf("Error in compiler: word not found %s\n", buf);
        }
    } else {
        vm->s = INTERPRET;
        vm->ip = vm->ip - 1;
        vm->psp = 0;
        vm->rsp = 0;
    }
}

void _deb(VM *vm) {
    printf("IP: %i  HP: %i  LP: %i\n", vm->ip, vm->hp, vm->lp);
    printf("PS: %6i\tRS: %6i\n", vm->psp, vm->rsp);
    for(int i = 0; i < (vm->psp > vm->rsp ? vm->psp : vm->rsp); ++i) {
        if(vm->psp > i)
            printf("%10i\t", vm->ps[i]);
        else
            printf("          \t");
        if(vm->rsp > i)
            printf("%10i\n", vm->rs[i]);
        else
            printf("          \n");
    }
    printf("\n\n");
}
*/


cell opcode(VM *vm) {
	cell addr = *((cell *) &(vm->mem[vm->ip]));
	vm->ip += CELL_SIZE;
    return addr;
}

void exec(VM *vm, cell addr) {
    switch(addr) {
		case NOP: _nop(vm); break;
		case LIT: _lit(vm); break;
		case HALT: _halt(vm); break;
		case DUP: _dup(vm); break;
		case DROP: _drop(vm); break;
		case SWAP: _swap(vm); break;
		case PUSH: _push(vm); break;
		case POP: _pop(vm); break;
		case PICKP: _pickp(vm); break;
		case PICKR: _pickr(vm); break;
		case JMP: _jmp(vm); break;
		case JZ: _jz(vm); break;
		case RET: _ret(vm); break;
		case EQ: _eq(vm); break;
		case NEQ: _neq(vm); break;
		case LT: _lt(vm); break;
		case GT: _gt(vm); break;
		case AND: _and(vm); break;
		case OR: _or(vm); break;
		case XOR: _xor(vm); break;
		case SHR: _shr(vm); break;
		case SHL: _shl(vm); break;
		case ADD: _add(vm); break;
		case SUB: _sub(vm); break;
		case MUL: _mul(vm); break;
		case DIV: _div(vm); break;
		case MOD: _mod(vm); break;
		case LDC: _ldc(vm); break;
		case STRC: _strc(vm); break;
		case LDB: _ldb(vm); break;
		case STRB: _strb(vm); break;
		case LDS: _lds(vm); break;
		case STRS: _strs(vm); break;
		case LDP: _ldp(vm); break;
		case STRP: _strp(vm); break;
		case LDR: _ldr(vm); break;
		case STRR: _strr(vm); break;
		case LDH: _ldh(vm); break;
		case STRH: _strh(vm); break;
		case LDL: _ldl(vm); break;
		case STRL: _strl(vm); break;
		case CSZ: _csz(vm); break;
		case CFUN: _cfun(vm); break;
		case KEY: _key(vm); break;
		case EMIT: _emit(vm); break;
/*
		case COL: _col(vm); break;
		case SEMI: _semi(vm); break;
		case INTERP: _interp(vm); break;
		case DEB: _deb(vm); break;
*/
    	default:
    		vm->rs[vm->rsp++] = vm->ip;
    		vm->ip = addr;
    }
}

void cycle(VM *vm) {
	exec(vm, opcode(vm));
}

void run(VM *vm) {
    while(vm->p == ON)
        cycle(vm);
}


void init(VM *vm) {
	
	int i;

    vm->p = ON;
    vm->s = INTERPRET;
    vm->i = stdin;
    vm->o = stdout;

    for(i = 0; i < 0x100; ++i)
        vm->ps[i] = 0;
    vm->psp = 0;

    for(i = 0; i < 0x100; ++i)
        vm->rs[i] = 0;
    vm->rsp = 0;

    for(i = 0; i < MEM_SIZE; ++i)
        vm->mem[i] = 0;

    vm->ip = 0;
    vm->hp = 0;
    vm->lp = 0;
}


/*
void word(VM *vm, char *name, char *fun, int len, char flag) {

    *((cell *) &(vm->mem[vm->hp])) = *((cell *) &(vm->lp));
    vm->lp = vm->hp;
    vm->hp += CELL_SIZE;

    vm->mem[vm->hp++] = strlen(name) | flag;

    for(unsigned int i = 0; i < strlen(name); ++i)
        vm->mem[vm->hp + i] = name[i] - (name[i] >= 'a' && name[i] <= 'z' ? 'a' - 'A' : 0);
    vm->hp += strlen(name);

    for(int i = 0; i < len; ++i) {
        vm->mem[vm->hp + i] = fun[i];
    }
    vm->hp += len;

    vm->mem[vm->hp++] = RET;
    

	*((cell *) &(vm->mem[vm->hp])) = INTERP;
	vm->hp += CELL_SIZE;
    *((cell *) &(vm->mem[vm->hp])) = HALT;
    vm->hp += CELL_SIZE;

}

void words(VM *vm) {

    char col_arr[] = {
        COL,
    };
    word(vm, ":", col_arr, sizeof(col_arr), MASK_VIS);

    char semi_arr[] = {
        SEMI,
    };
    word(vm, ";", semi_arr, sizeof(semi_arr), MASK_VIS | MASK_IMM);

    char app_arr[] = {
        LDH, STRB, LDH, LIT, 1, 0, ADD, STRH,
    };
    word(vm, "app", app_arr, sizeof(app_arr), MASK_VIS | MASK_IMM);

}
*/

void debug(VM *vm) {
	int i;
    printf("Debug Info\n");
    printf("Power: %s\n", vm->p == OFF ? "OFF" : "ON");
    printf("State: %s\n", vm->s == INTERPRET ? "INTERPRET" : "COMPILE");

    printf("PS: %6i\tRS: %6i\n", vm->psp, vm->rsp);
    for(i = 0; i < (vm->psp > vm->rsp ? vm->psp : vm->rsp); ++i) {
        if(vm->psp > i)
            printf("%10i\t", vm->ps[i]);
        else
            printf("          \t");
        if(vm->rsp > i)
            printf("%10i\n", vm->rs[i]);
        else
            printf("          \n");
    }

    printf("IP: %i  HP: %i  LP: %i\n", vm->ip, vm->hp, vm->lp);
    for(i = 0; i < vm->hp; ++i)
        printf("0x%04x: %3i %c\n", i, vm->mem[i], isgraph(vm->mem[i]) ? vm->mem[i] : '_');

    printf("\n\n");
}


void save(VM *vm, char *file) {
	int i;
    FILE *fp = fopen(file, "wb");
    for(i = 0; i < vm->hp; ++i)
        putc(vm->mem[i], fp);
    fclose(fp);
}

void restore(VM *vm, char *file) {
    int c;
    FILE *fp = fopen(file, "rb");
    while((c = getc(fp)) != EOF)
        vm->mem[vm->hp++] = c;
    fclose(fp);
}

void dump(VM *vm, char *file) {
	int i;
    FILE *fp = fopen(file, "w");
    fprintf(fp, "{ ");
    for(i = 0; i < vm->hp; ++i) {
        if(i % 8 == 0)
            fprintf(fp, "\n");
        fprintf(fp, "%#05x, ", vm->mem[i]);
    }
    fprintf(fp, "\n};");

}







