Maybe divide VM structure into memory and stacks
so memory can be shared between different programs

typedef byte mem[MEM_SIZE];
typedef struct spu spu;
struct spu {
    power p;
    cell ip;

    cell ps[0x100];
    byte psp;

    cell rs[0x100];
    byte rsp;
};

typedef struct vm vm;
struct vm {
    mem *m;
    spu *s;
};


Specification for ff forth interpreter:

Level 1: Minimal Word Set
As long as the required words are available, different implementations should work equally.
The following words are the minimal required amount of words to bootstrap the rest of the system.

Stack:          DUP DROP SWAP R< R> (PUSH POP) P@ R@ (PICK RICK)
Control flow:   JMP JZ
Memory:         @ ! (FETCH STORE) C@ C! (CHAR-FETHC CHAR-STORE) CELL MEM
Logic:          = <> > < (EQ NEQ GT LT) AND OR XOR TRUE FALSE << >> (SHR SHL)
Arithmetic:     + - * / % (ADD SUB MUL DIV MOD)
IO:             KEY EMIT
Special:        STATE@ STATE! PSP@ PSP! RSP@ RSP! HP@ HP! LP@ LP!
Definitions:    : ; IMMEDIATE LIT '

Level 2: Required Word Set
*** some of these words are implementation specific???

: [ FALSE STATE! ; IMMEDIATE
: ] TRUE STATE! ;

: POSTPONE ' , ; IMMEDIATE \ append next word as a function
: LITERAL [ ' LIT DUP , , ] , , ; IMMEDIATE \ append whatever is on the stack as a literal
: ['] ' POSTPONE LITERAL ; IMMEDIATE

Also needed:
, C,

Also usefull:
+!
: ROT >R SWAP R> SWAP ;

Level 3: Required VM and Opcodes

Opcodes:

