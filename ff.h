#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


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
    COL, SEMI, EVAL, DEB,
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
	vm->hp += sizeof(cell);
	
	byte i;
	for(i = 0; buf[i] != '\0'; ++i)
		vm->mem[vm->hp + 1 + i] = buf[i];

	vm->mem[vm->hp] = i;
	vm->hp += 1 + i;
}

void _semi(VM *vm) {
	vm->mem[vm->hp++] = RET;
	vm->mem[vm->lp + sizeof(cell)] |= MASK_VIS;
	vm->s = INTERPRET;
}

void _eval(VM *vm) {

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
	
	ptr addr;
	byte flags;
	
	for(addr = vm->lp; addr != 0; addr = *((ptr *) &(vm->mem[addr]))) {
		flags = vm->mem[addr + sizeof(cell)];
		if((flags & MASK_VIS) && len == (flags & WORD_LEN)) {
			if(strncmp(buf, (char *) &(vm->mem[addr + sizeof(cell) + 1]), len) == 0)
				break;
		}
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
    		vm->ip = addr + sizeof(cell) + 1 + len;
		} else if(nflag) {
			vm->ps[vm->psp++] = num;
			vm->ip = vm->ip - 1;
		} else {
			vm->ip = vm->ip - 1;
		}
	} else if(vm->s == COMPILE) {
		if(addr) {
			if(flags & MASK_IMM) {
				vm->rs[vm->rsp++] = vm->ip - 1;
    			vm->ip = addr + sizeof(cell) + 1 + len;
			} else {
				vm->mem[vm->hp++] = LIT;
				*((cell *) &(vm->mem[vm->hp])) = addr + sizeof(cell) + 1 + len;
				vm->hp += sizeof(cell);
				vm->mem[vm->hp++] = CALL;
				vm->ip = vm->ip - 1;
			}
		} else if(nflag) {
			vm->mem[vm->hp++] = LIT;
			*((cell *) &(vm->mem[vm->hp])) = num;
			vm->hp += sizeof(cell);
			vm->ip = vm->ip - 1;
		} else {
			vm->s = INTERPRET;
			vm->hp = vm->lp;
			vm->lp = *((ptr *) &(vm->mem[vm->lp]));
			vm->ip = vm->ip - 1;
		}
	} else {
		vm->s = INTERPRET;
		vm->ip = vm->ip - 1;
	}
}

void _deb(VM *vm) {
	printf("PS: %6i\tRS: %6i\n", vm->psp, vm->rsp);
    for(int i = 0; i < (vm->psp > vm->rsp ? vm->psp : vm->rsp); ++i)
        printf("%10i\t%10i\n", vm->ps[i], vm->rs[i]);
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
    _col, _semi, _eval, _deb,
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
        
    vm->mem[0] = EVAL;
    vm->mem[1] = HALT;
        
    vm->ip = 0;
    vm->hp = 2;
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


void word(VM *vm, char *name, char *fun, int len, char flag) {
	
	*((cell *) &(vm->mem[vm->hp])) = *((cell *) &(vm->lp));
	vm->lp = vm->hp;
	vm->hp += sizeof(cell);
	
	vm->mem[vm->hp++] = strlen(name) | flag;
	
	for(int i = 0; i < strlen(name); ++i)
		vm->mem[vm->hp + i] = name[i] - (name[i] >= 'a' && name[i] <= 'z' ? 'a' - 'A' : 0);
	vm->hp += strlen(name);
	
	for(int i = 0; i < len; ++i) {
		vm->mem[vm->hp + i] = fun[i];
	}
	vm->hp += len;
	
	vm->mem[vm->hp++] = RET;
}

void words(VM *vm) {

	char bapp_arr[] = {
		LDH, STRB, LDH, LIT, 1, 0, ADD, STRH,
	};
	word(vm, "bapp", bapp_arr, sizeof(bapp_arr), MASK_VIS | MASK_IMM);
	
	char capp_arr[] = {
		LDH, STRC, LDH, LIT, 2, 0, ADD, STRH,
	};
	word(vm, "capp", capp_arr, sizeof(capp_arr), MASK_VIS | MASK_IMM);
	
	char col_arr[] = {
		COL,
	};
	word(vm, ":", col_arr, sizeof(col_arr), MASK_VIS);
	
	char semi_arr[] = {
		SEMI,
	};
	word(vm, ";", semi_arr, sizeof(semi_arr), MASK_VIS | MASK_IMM);

}


void debug(VM *vm) {
    printf("Debug Info\n");
    printf("Power: %s\n", vm->p == OFF ? "OFF" : "ON");
    printf("State: %s\n", vm->s == INTERPRET ? "INTERPRET" : "COMPILE");

    printf("PS: %6i\tRS: %6i\n", vm->psp, vm->rsp);
    for(int i = 0; i < (vm->psp > vm->rsp ? vm->psp : vm->rsp); ++i)
        printf("%10i\t%10i\n", vm->ps[i], vm->rs[i]);

    printf("IP: %i  HP: %i  LP: %i\n", vm->ip, vm->hp, vm->lp);
    for(int i = 0; i < vm->hp; ++i)
        printf("0x%04x: %3i %c\n", i, vm->mem[i], isgraph(vm->mem[i]) ? vm->mem[i] : '_');

    printf("\n\n");
}


void save(VM *vm, char *file) {
	FILE *fp = fopen(file, "wb");
	for(int i = 0; i < vm->hp; ++i)
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








