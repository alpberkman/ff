

#ifndef FF_IMPL
#define FF_IMPL

#define CELL_TYPE short
#define BYTE_TYPE unsigned char
#define MEM_SIZE (0x8000)


#define FFALSE (0)
#define TTRUE (-1)

#define MASK_VIS (1<<7)
#define MASK_IMM (1<<6)
#define WORD_LEN (31)


#define CELL_SIZE ((cell) sizeof(cell))
#define BYTE_SIZE ((cell) sizeof(byte))


typedef CELL_TYPE cell;
typedef BYTE_TYPE byte;

typedef struct VM VM;
typedef enum power power;
typedef struct SPU SPU;
typedef byte *RAM;

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
void _byte(VM *vm);
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


enum power {
    OFF = FFALSE,
    ON = TTRUE,
};

struct SPU {
    power p;

    cell ip;

    cell ps[0x100];
    byte psp;

    cell rs[0x100];
    byte rsp;
};

struct VM {
    SPU spu;
    RAM ram;
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
CELL, BYTE,
MEM,
// Special
LDP, STRP,
LDR, STRR,
LDI, STRI,
// IO
KEY, EMIT,
// Call
CALL,
// Final Enum
FF,
};

#endif



