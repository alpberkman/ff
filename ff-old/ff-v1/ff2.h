#include <stdint.h>
#include <limits.h>
#include <stdio.h>

#define TRUE -1
#define FALSE 0

#define PSTACK_SIZE (0x100)
#define RSTACK_SIZE (0x100)
#define MEMORY_SIZE (0x10000)


typedef struct VM VM;
typedef enum power power;
typedef enum error error;
typedef enum state state;
typedef void (*fun)(VM *vm);

#if CELL_SIZE == 4
typedef int32_t cell;
typedef uint32_t ptr;
typedef uint8_t byte; 
#elif CELL_SIZE == 2
typedef int16_t cell;
typedef uint16_t ptr;
typedef uint8_t byte;
#else
typedef int32_t cell;
typedef uint32_t ptr;
typedef uint8_t byte; 
#endif

typedef enum op op;


enum power {
    OFF,
    ON,
};

enum error {
	NO_ERR,
    PS_UNDERFLOW,
    PS_OVERFLOW,
    RS_UNDERFLOW,
    RS_OVERFLOW,
    MEM_OUT_OF_BOUNDS,
    JMP_ADDR_OUT_OF_BOUNDS,
    NOT_A_WORD,
    DIV_BY_ZERO,
};

enum state {
    INTERPRET,
    COMPILE,
};


struct VM {
	power p;
	error e;
	fun err_ctrl;
	state s;
	fun prims[256];

	cell ps[PSTACK_SIZE];
	ptr psp;

	cell rs[RSTACK_SIZE];
	ptr rsp;

	byte mem[MEMORY_SIZE];
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
	LDH, STRH,
	LDL, STRL,
	
	KEY, EMIT,	
};


void _nop(VM *vm);
void _lit(VM *vm);
void _halt(VM *vm);

void _dup(VM *vm);
void _drop(VM *vm);
void _swap(VM *vm);
void _push(VM *vm);
void _pop(VM *vm);
void _pickp(VM *vm);
void _pickr(VM *vm);

void _jmp(VM *vm);
void _jz(VM *vm);

void _call(VM *vm);
void _ret(VM *vm);

void _eq(VM *vm);
void _neq(VM *vm);
void _lt(VM *vm);
void _gt(VM *vm);

void _and(VM *vm);
void _or(VM *vm);
void _xor(VM *vm);
void _shr(VM *vm);
void _shl(VM *vm);

void _add(VM *vm);
void _sub(VM *vm);
void _mul(VM *vm);
void _div(VM *vm);
void _mod(VM *vm);

void _ldc(VM *vm);
void _strc(VM *vm);
void _ldb(VM *vm);
void _strb(VM *vm);

void _lds(VM *vm);
void _strs(VM *vm);
void _ldp(VM *vm);
void _strp(VM *vm);
void _ldr(VM *vm);
void _strr(VM *vm);
void _ldh(VM *vm);
void _strh(VM *vm);
void _ldl(VM *vm);
void _strl(VM *vm);

void _key(VM *vm);
void _emit(VM *vm);

void init(VM *vm);
void exec(VM *vm);
void run(VM *vm);

void debug(VM *vm);







