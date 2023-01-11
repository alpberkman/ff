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
    vm->mem[0] = LIT;
    vm->mem[1] = 4;
    vm->mem[2] = NOP;
    vm->mem[3] = CALL;
    vm->mem[4] = HALT;
        
    vm->ip = 5;
    vm->hp = 5;
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


void save(VM *vm, char *file) {
	FILE *fp = open(file, "wb");
	for(int i = 0; i < vm->hp; ++i)
		putc(fp, vm->mem[i]);
	close(fp);
}

void restore(VM *vm, char *file) {
	int c;
	FILE *fp = open(file, "rb");
	while((c = getc(fp)) != EOF)
		vm->mem[vm->hp++] = c;
	close(fp);
}


#define MASK_VIS (1<<7)
#define MASK_IMM (1<<6)
#define MASK_LEN (0b00011111)
#include <string.h>
void word(VM *vm, char *name, char *fun, int len, char flag) {
	
	memcpy(vm->mem + vm->hp, &(vm->lp), CELL_SIZE);
	vm->lp = vm->hp;
	vm->hp += CELL_SIZE;
	
	vm->mem[vm->hp++] = strlen(name) | flag;
	
	memcpy(vm->mem + vm->hp, name, strlen(name));//Make name capital
	vm->hp += strlen(name);
	
	memcpy(vm->mem + vm->hp, fun, len);
	vm->hp += len;
	
	vm->mem[vm->hp++] = RET;
}

// How about pushm memory push
#define APP LDH, STRB, LDH, LIT, 1, 0, ADD, STRH
#define BEQ(X) LIT, X, 0, EQ
#define ISSPACE LIT, 0, 0, \
				OVER, BEQ(' '), OR, \
				OVER, BEQ('\t'), OR, \
				OVER, BEQ('\n'), OR, \
				OVER, BEQ('\v'), OR, \
				OVER, BEQ('\f'), OR, \
				OVER, BEQ('\r'), OR
#define NOT LIT, 0, 0, NEQ
					
void words(VM *vm) {

	char app_arr[] = {
		APP
	};
	word(vm, "app", app_arr, sizeof(app_arr), MASK_VIS | MASK_IMM);

	char semi_colon_arr[] = {
		LIT, RET, 0, APP,
		LDL, LIT, CELL_SIZE, 0, ADD, DUP, LDB,
		LIT, MASK_VIS, 0, OR, SWAP, STRB
	};
	word(vm, "semi_colon", semi_colon_arr, sizeof(semi_colon_arr), MASK_VIS | MASK_IMM);
	
	char colon_arr[] = {
		LIT, COMPILE, 0, STRS,
		DUP,
		DROP, KEY, ISSPACE,
		NOT, LIT, /*addr drop before key*/,JZ,
		APP, LIT, 1, 0,
		
		
		
	};
	word(vm, "colon", colon_arr, sizeof(colon_arr), MASK_VIS);
	
	char repl_arr[] = {
		
	};
	word(vm, "repl", repl_arr, sizeof(repl_arr), MASK_VIS);
	
	// ' tick
	//
	
	
/*
	char immediate_arr[] = {
		LDL, LIT, CELL_SIZE, 0, ADD, DUP, LDB,
		LIT, (1<<6), 0, OR, SWAP, STRB
	};
	word(vm, "immediate", immediate_arr, sizeof(immediate_arr), MASK_VIS);
	
	
	char nop_arr[] = {
		LIT, NOP, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "nop", nop_arr, sizeof(nop_arr), MASK_VIS | MASK_IMM);
	
	char lit_arr[] = {
		LIT, LIT, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "lit", nop_arr, sizeof(lit_arr), MASK_VIS | MASK_IMM);
	
	char halt_arr[] = {
		LIT, HALT, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "halt", halt_arr, sizeof(halt_arr), MASK_VIS | MASK_IMM);
	
	
	char dup_arr[] = {
		DUP
	};
	word(vm, "dup", dup_arr, sizeof(dup_arr), MASK_VIS);
	
	char drop_arr[] = {
		DROP
	};
	word(vm, "drop", drop_arr, sizeof(drop_arr), MASK_VIS);
	
	char swap_arr[] = {
		SWAP
	};
	word(vm, "swap", swap_arr, sizeof(swap_arr), MASK_VIS);
	
	char push_arr[] = {
		PUSH
	};
	word(vm, "push", push_arr, sizeof(push_arr), MASK_VIS);
	
	char pop_arr[] = {
		POP
	};
	word(vm, "pop", pop_arr, sizeof(pop_arr), MASK_VIS);
	
	char pickp_arr[] = {
		PICKP
	};
	word(vm, "pickp", pickp_arr, sizeof(pickp_arr), MASK_VIS);
	
	char pickr_arr[] = {
		PICKR
	};
	word(vm, "pickr", pickr_arr, sizeof(pickr_arr), MASK_VIS);
	
	
	char jmp_arr[] = {
		LIT, JMP, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "jmp", jmp_arr, sizeof(jmp_arr), MASK_VIS | MASK_IMM);
	
	char jz_arr[] = {
		LIT, JZ, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "jz", jz_arr, sizeof(jz_arr), MASK_VIS | MASK_IMM);
	
	
	char call_arr[] = {
		LIT, CALL, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "call", call_arr, sizeof(call_arr), MASK_VIS | MASK_IMM);
	
	char ret_arr[] = {
		LIT, RET, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "ret", ret_arr, sizeof(ret_arr), MASK_VIS | MASK_IMM);
	
	
	char eq_arr[] = {
		EQ
	};
	word(vm, "eq", eq_arr, sizeof(eq_arr), MASK_VIS);
	
	char neq_arr[] = {
		NEQ
	};
	word(vm, "neq", neq_arr, sizeof(neq_arr), MASK_VIS);
	
	char lt_arr[] = {
		LT
	};
	word(vm, "lt", lt_arr, sizeof(lt_arr), MASK_VIS);
	
	char gt_arr[] = {
		GT
	};
	word(vm, "gt", gt_arr, sizeof(gt_arr), MASK_VIS);
	
	
	
	char _arr[] = {
		
	};
	word(vm, "", _arr, sizeof(_arr), MASK_VIS);


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
	*/
}

void repl(VM *vm) {
	char buf[MAX_LEN + 1];
	while(scanf("%31s", buf) != EOF) {
		if(vm->s == INTERPRET)
			interp(vm, buf);
		else
			comp(vm, buf);
		run(vm);
		vm->p = ON;
	}
}

void interp(VM *vm, char *word) {}
void comp(VM *vm, char *word) {}
























