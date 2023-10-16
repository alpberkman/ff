#include "ff.h"
#include <string.h>
#include <ctype.h>

void stacks(VM *vm);

cell hp;
cell lp;


void m_header(VM *vm, char *name, unsigned char flag) {
    cell i;

    *((cell *) &(vm->mem[hp])) = *((cell *) &(lp));
    lp = hp;
    hp += CELL_SIZE;

    vm->mem[hp++] = strlen(name) | flag;

    for(i = 0; i < (cell) strlen(name); ++i)
        vm->mem[hp + i] = toupper(name[i]);
    hp += strlen(name);
}
void m_tail() {}
void m_opcodes(VM *vm, char *opcodes, int size) {
    for(int i = 0; i < size; ++i)
        vm->mem[hp+i] = opcodes[i];
    hp += size;
}
void m_literal(VM *vm, cell number) {
    *((cell *) &(vm->mem[hp])) = LIT;
    hp += CELL_SIZE;
    *((cell *) &(vm->mem[hp])) = number;
    hp += CELL_SIZE;
}
void m_alloc(VM *vm, cell number) {
    hp += number;
    (void) vm;
}
void m_word(VM *vm, char *word) {
    cell addr;
    byte flags;
    cell len = strlen(word);
    char buf[32] = {0};
    //puts(word);
    for(cell i = 0; i < strlen(word); ++i)
        buf[i] = toupper(word[i]);
    //puts(buf);

    for(addr = lp; addr != 0; addr = *((cell *) &(vm->mem[addr]))) {
        flags = vm->mem[addr + CELL_SIZE];
        if((flags & MASK_VIS) && len == (flags & WORD_LEN))
            if(strncmp(buf, (char *) &(vm->mem[addr + CELL_SIZE + 1]), len) == 0)
                break;
    }

    if(addr == 0) {
        printf("Error in m_word %s at %i\n", buf, addr);
        return;
    }

    *((cell *) &(vm->mem[hp])) =  addr + CELL_SIZE + 1 + len;
    hp += CELL_SIZE;
}
void m_num(VM *vm, cell num) {
    m_word(vm, "LIT");
    *((cell *) &(vm->mem[hp])) =  num;
    hp += CELL_SIZE;
}
void m_var(VM *vm, char *name) {
    cell tmp = hp;
    *((cell *) &(vm->mem[hp])) = 0;
    m_alloc(vm, CELL_SIZE);
    m_header(vm, name, MASK_VIS);
    m_num(vm, tmp);
    m_word(vm, "EXIT");
}
void m_const(VM *vm, char *name, cell num) {
    m_header(vm, name, MASK_VIS);
    m_num(vm, num);
    m_word(vm, "EXIT");
}
/*
void m_if(VM *vm) {
    m_word(vm, "(if)");

}
void m_else(VM *vm) {}
void m_then(VM *vm) {}

: [IF] [ 5 CELLS ] LITERAL IP + JZ R> CELL+ JMP R> @ JMP ;
: IF ['] [IF] , HERE HERE CELL+ HERE! ; IMMEDIATE


: [THEN] ;
: THEN ['] [THEN] , HERE SWAP ! ; IMMEDIATE


: [ELSE] R> @ JMP ;
: ELSE ['] [ELSE] , >R HERE HERE CELL+ HERE! R> HERE SWAP ! ; IMMEDIATE

void m_do(VM *vm) {}
void m_ploop(VM *vm) {}
void m_loop(VM *vm) {}

void m_begin(VM *vm) {}
void m_again(VM *vm) {}
void m_until(VM *vm) {}
void m_while(VM *vm) {}
void m_repeat(VM *vm) {}
*/

#define STR(X) #X
#define S(X) STR(X)

#define HEADER(name, flag) m_header(vm, STR(name), (flag));
#define OPCODES(ops) m_opcodes(vm, (char *) ops, sizeof(ops));
#define LITERAL(num) m_literal(vm, num);
#define ALLOC(num) m_alloc(vm, num);
#define WORD(name) m_word(vm, STR(name));
#define NUMBER(num) m_num(vm, num);
#define VAR(name) m_var(vm, STR(name));
#define CONST(name, num) m_const(vm, STR(name), num);
//#define entry(name, entry) HEADER(name, entry) OPCODES()

void init(VM *vm) {


    cell tmp, tmp0, tmp1, tmp2, tmp3;


    for(int i = 0; i < MEM_SIZE; ++i)
        vm->mem[i] = 0;

    ALLOC(16+256);


    // Special
    cell op_nop[] = {
        RET,
    };
    cell op_lit[] = {
        POP, DUP, CELL, ADD, PUSH, LDC, RET,
    };
    cell op_halt[] = {
        HALT,
    };

    HEADER(nop, MASK_VIS)
    OPCODES(op_nop);

    HEADER(lit, MASK_VIS);
    OPCODES(op_lit);

    HEADER(halt, MASK_VIS);
    OPCODES(op_halt);


    // Stack
    cell op_dup[] = {
        DUP, RET,
    };
    cell op_drop[] = {
        DROP, RET,
    };
    cell op_swap[] = {
        SWAP, RET,
    };
    cell op_push[] = {
        POP, SWAP, PUSH, PUSH, RET,
    };
    cell op_pop[] = {
        POP, POP, SWAP, PUSH, RET,
    };
    cell op_pick[] = {
        PICK, RET,
    };
    cell op_rick[] = {
        LIT, 1, ADD, RICK, RET,
    };

    HEADER(dup, MASK_VIS);
    OPCODES(op_dup);

    HEADER(drop, MASK_VIS);
    OPCODES(op_drop);

    HEADER(swap, MASK_VIS);
    OPCODES(op_swap);

    HEADER(R<, MASK_VIS);
    OPCODES(op_push);

    HEADER(R>, MASK_VIS);
    OPCODES(op_pop);

    HEADER(pick, MASK_VIS);
    OPCODES(op_pick);

    HEADER(rick, MASK_VIS);
    OPCODES(op_rick);


    // Controlflow
    cell op_jmp[] = {
        POP, DROP, JMP,
    };
    cell op_jz[] = {
        PUSH,
        CELL, LIT, 6, MUL, LDI, ADD, JZ,
        POP, DROP, POP, JMP,
        POP, POP, DROP, JMP,
    };
    cell op_ret[] = {
        POP, DROP, RET,
    };

    HEADER(jmp, MASK_VIS);
    OPCODES(op_jmp);

    HEADER(jz, MASK_VIS);
    OPCODES(op_jz);

    HEADER(exit, MASK_VIS);
    OPCODES(op_ret);


    // Logic
    cell op_eq[] = {
        EQ, RET,
    };
    cell op_neq[] = {
        NEQ, RET,
    };
    cell op_gt[] = {
        GT, RET,
    };
    cell op_lt[] = {
        LT, RET,
    };
    cell op_and[] = {
        AND, RET,
    };
    cell op_or[] = {
        OR, RET,
    };
    cell op_xor[] = {
        XOR, RET,
    };
    cell op_shr[] = {
        SHR, RET,
    };
    cell op_shl[] = {
        SHL, RET,
    };
    cell op_tru[] = {
        TRU, RET,
    };
    cell op_fls[] = {
        FLS, RET,
    };

    HEADER(=, MASK_VIS);
    OPCODES(op_eq);

    HEADER(<>, MASK_VIS);
    OPCODES(op_neq);

    HEADER(>, MASK_VIS);
    OPCODES(op_gt);

    HEADER(<, MASK_VIS);
    OPCODES(op_lt);

    HEADER(and, MASK_VIS);
    OPCODES(op_and);

    HEADER(or, MASK_VIS);
    OPCODES(op_or);

    HEADER(xor, MASK_VIS);
    OPCODES(op_xor);

    HEADER(>>, MASK_VIS);
    OPCODES(op_shr);

    HEADER(<<, MASK_VIS);
    OPCODES(op_shl);

    HEADER(true, MASK_VIS);
    OPCODES(op_tru);

    HEADER(false, MASK_VIS);
    OPCODES(op_fls);


    // Arithmetic
    cell op_add[] = {
        ADD, RET,
    };
    cell op_sub[] = {
        SUB, RET,
    };
    cell op_mul[] = {
        MUL, RET,
    };
    cell op_div[] = {
        DIV, RET,
    };
    cell op_mod[] = {
        MOD, RET,
    };

    HEADER(+, MASK_VIS);
    OPCODES(op_add);

    HEADER(-, MASK_VIS);
    OPCODES(op_sub);

    HEADER(*, MASK_VIS);
    OPCODES(op_mul);

    HEADER(/, MASK_VIS);
    OPCODES(op_div);

    HEADER(%, MASK_VIS);
    OPCODES(op_mod);


    // Memory
    cell op_ldc[] = {
        LDC, RET,
    };
    cell op_strc[] = {
        STRC, RET,
    };
    cell op_ldb[] = {
        LDB, RET,
    };
    cell op_strb[] = {
        STRB, RET,
    };
    cell op_cell[] = {
        CELL, RET,
    };
    cell op_mem[] = {
        MEM, RET,
    };

    HEADER(@, MASK_VIS);
    OPCODES(op_ldc);

    HEADER(!, MASK_VIS);
    OPCODES(op_strc);

    HEADER(C@, MASK_VIS);
    OPCODES(op_ldb);

    HEADER(C!, MASK_VIS);
    OPCODES(op_strb);

    HEADER(CELL, MASK_VIS);
    OPCODES(op_cell);

    HEADER(MEM, MASK_VIS);
    OPCODES(op_mem);


    // Special
    cell op_ldp[] = {
        LDP, RET,
    };
    cell op_strp[] = {
        STRP, RET,
    };
    cell op_ldr[] = {
        LDR, RET,
    };
    cell op_strr[] = {
        STRR, RET,
    };
    cell op_ldi[] = {
        POP, DUP, PUSH, RET,
    };
    cell op_stri[] = {
        STRI, RET,
    };

    HEADER(PSP@, MASK_VIS);
    OPCODES(op_ldp);

    HEADER(PSP!, MASK_VIS);
    OPCODES(op_strp);

    HEADER(RSP@, MASK_VIS);
    OPCODES(op_ldr);

    HEADER(RSP!, MASK_VIS);
    OPCODES(op_strr);

    HEADER(IP@, MASK_VIS);
    OPCODES(op_ldi);

    HEADER(IP!, MASK_VIS);
    OPCODES(op_stri);


    // IO
    cell op_key[] = {
        KEY, RET,
    };
    cell op_emit[] = {
        EMIT, RET,
    };

    HEADER(key, MASK_VIS);
    OPCODES(op_key);

    HEADER(emit, MASK_VIS);
    OPCODES(op_emit);


    // Call
    cell op_call[] = {
        POP, DROP, PUSH, RET,
    };

    HEADER(CALL, MASK_VIS);
    OPCODES(op_call);


    HEADER(XXXXXXXXXXXXXX, MASK_VIS);
    WORD(EXIT);


    // High level words
    // State & friends

/*
    tmp = hp;
    ALLOC(CELL_SIZE);
    HEADER(STATE, MASK_VIS);
    NUMBER(tmp);
    WORD(EXIT);
*/
    VAR(STATE);

    HEADER(STATE@, MASK_VIS);
    WORD(STATE);
    WORD(@);
    WORD(EXIT);

    HEADER(STATE!, MASK_VIS);
    WORD(STATE);
    WORD(!);
    WORD(EXIT);

    HEADER([, MASK_VIS | MASK_IMM);
    WORD(false);
    WORD(STATE!);
    WORD(EXIT);

    HEADER(], MASK_VIS);
    WORD(true);
    WORD(STATE!);
    WORD(EXIT);

    HEADER(CELLS, MASK_VIS);
    WORD(CELL);
    WORD(*);
    WORD(EXIT);

    HEADER(CELL+, MASK_VIS);
    WORD(CELL);
    WORD(+);
    WORD(EXIT);

    HEADER(+!, MASK_VIS);
    WORD(SWAP);
    NUMBER(1);
    WORD(PICK);
    WORD(@);
    WORD(+);
    WORD(SWAP);
    WORD(!);
    WORD(EXIT);


    VAR(READ-CTR);

    tmp = hp;
    ALLOC(32);
    CONST(READ-BUF, tmp);

    HEADER(READ-APPEND, MASK_VIS);
    WORD(READ-BUF);
    WORD(READ-CTR);
    WORD(@);
    WORD(+);
    WORD(!);
    NUMBER(1);
    WORD(READ-CTR);
    WORD(+!);
    WORD(EXIT);

    HEADER(READ, MASK_VIS);
    NUMBER(0);
    WORD(READ-CTR);
    WORD(!);
    WORD(EXIT);


// if then else
    HEADER([IF], MASK_VIS);
    HEADER(IF, MASK_VIS | MASK_IMM);


/*
: [IF] [ 5 CELLS ] LITERAL IP + JZ R> CELL+ JMP R> @ JMP ;
: IF ['] [IF] , HERE HERE CELL+ HERE! ; IMMEDIATE


: [THEN] ;
: THEN ['] [THEN] , HERE SWAP ! ; IMMEDIATE


: [ELSE] R> @ JMP ;
: ELSE ['] [ELSE] , >R HERE HERE CELL+ HERE! R> HERE SWAP ! ; IMMEDIATE

*/


    HEADER(READ, MASK_VIS);
    WORD(KEY);
    WORD(EMIT);
    WORD(EXIT);

/*

READ-APPEND:
READ-BUF READ-CTR @ + !
1 READ-CTR +!

READ:
0 READ-CTR !
BEGIN
    KEY DUP ISSPACE
WHILE
    DROP
REPEAT
READ-APPEND
BEGIN
    KEY
    DUP ISSPACE IF DROP EXIT THEN
    READ-APPEND
    READ-CTR @ 30 31 =
UNTIL
*/


    // XXXXXXXXXXX
    HEADER(TEST, 0);
    vm->ip = hp;

    WORD(KEY);
    WORD(READ-APPEND);
    WORD(KEY);
    WORD(READ-APPEND);
    WORD(KEY);
    WORD(READ-APPEND);
    WORD(KEY);
    WORD(READ-APPEND);

    WORD(READ-CTR);
    WORD(@);

    WORD(HALT);
    run(vm);
    return;


    /*
    vm->ps[(vm->psp)++] = 4;
    vm->ps[(vm->psp)++] = 7;
    vm->rs[(vm->rsp)++] = 1;
    */


    //cell x[] = {LIT, 1, LIT, 3, ADD};

    //for(int i = 0; i < sizeof(x)/CELL_SIZE; i+=CELL_SIZE))
}

void disasm(VM *vm, cell addr, cell begin, cell end) {
    while(begin < end) {
        printf("%04x ", begin);

        switch(*((cell *) &(vm->mem[begin]))){
            case NOP: printf("NOP"); break;
            case LIT:
                printf("LIT");
                begin += CELL_SIZE;
                printf(" (%i)", *((cell *) &(vm->mem[begin])));
                printf("\n%04x %i/%04x", begin, *((cell *) &(vm->mem[begin])), *((cell *) &(vm->mem[begin])));
                break;
            case HALT: printf("HALT"); break;
            case DUP: printf("DUP"); break;
            case DROP: printf("DROP"); break;
            case SWAP: printf("SWAP"); break;
            case PUSH: printf("PUSH"); break;
            case POP: printf("POP"); break;
            case PICK: printf("PICK"); break;
            case RICK: printf("RICK"); break;
            case JMP: printf("JMP"); break;
            case JZ: printf("JZ"); break;
            case RET: printf("RET"); break;
            case EQ: printf("EQ"); break;
            case NEQ: printf("NEQ"); break;
            case GT: printf("GT"); break;
            case LT: printf("LT"); break;
            case AND: printf("AND"); break;
            case OR: printf("OR"); break;
            case XOR: printf("XOR"); break;
            case SHR: printf("SHR"); break;
            case SHL: printf("SHL"); break;
            case TRU: printf("TRU"); break;
            case FLS: printf("FLS"); break;
            case ADD: printf("ADD"); break;
            case SUB: printf("SUB"); break;
            case MUL: printf("MUL"); break;
            case DIV: printf("DIV"); break;
            case MOD: printf("MOD"); break;
            case LDC: printf("LDC"); break;
            case STRC: printf("STRC"); break;
            case LDB: printf("LDB"); break;
            case STRB: printf("STRB"); break;
            case CELL: printf("CELL"); break;
            case MEM: printf("MEM"); break;
            case LDP: printf("LDP"); break;
            case STRP: printf("STRP"); break;
            case LDR: printf("LDR"); break;
            case STRR: printf("STRR"); break;
            case LDI: printf("LDI"); break;
            case STRI: printf("STRI"); break;
            case KEY: printf("KEY"); break;
            case EMIT: printf("EMIT"); break;
            case CALL: printf("CALL"); break;
            default: {
                cell f = addr;
                while(*((cell *) &(vm->mem[begin])) < f)
                    f = *((cell *) &(vm->mem[f]));
                printf("%.*s (%04x)", (vm->mem[f + CELL_SIZE] & WORD_LEN), (char *) &(vm->mem[f + CELL_SIZE + 1]), *((cell *) &(vm->mem[begin])));
            }
        }
        printf("\n");

        begin += CELL_SIZE;
    }
    puts("");
}

void list1(VM *vm) {
    cell addr;
    cell end;
    byte flags;

    printf("NAME        LEN VIS IMM INSTR\n");
    printf("-----------------------------\n");
    for(addr = lp, end = hp; addr != 0; end = addr, addr = *((cell *) &(vm->mem[addr]))) {
        flags = vm->mem[addr + CELL_SIZE];
        printf("%.*s %.*s%i   %c   %c   %02i : ",
            (flags & WORD_LEN), (char *) &(vm->mem[addr + CELL_SIZE + 1]),
            12 - (flags & WORD_LEN), "                               ",
            (flags & WORD_LEN),
            (flags & MASK_VIS) ? '+' : '-',//"VIS" : "INV",
            (flags & MASK_IMM) ? '+' : '-',//"IMM" : "NIM",
            end - addr - CELL_SIZE - 1 - (flags & WORD_LEN)
        );
        for(int i = 0; i < (end - addr - CELL_SIZE - 1 - (flags & WORD_LEN))/CELL_SIZE; ++i)
            printf("%04x ", *((cell *) &(vm->mem[addr + CELL_SIZE + 1 + (flags & WORD_LEN) + i*CELL_SIZE])));
        puts("");
    }
    puts("\n\n");
}

void list2(VM *vm) {
    cell addr;
    cell end;
    byte flags;

    printf("NAME        LEN VIS IMM INSTR\n");
    printf("-----------------------------\n");
    for(addr = lp, end = hp; addr != 0; end = addr, addr = *((cell *) &(vm->mem[addr]))) {
        flags = vm->mem[addr + CELL_SIZE];
        printf("%.*s %.*s%i   %c   %c   %02i :\n",
            (flags & WORD_LEN), (char *) &(vm->mem[addr + CELL_SIZE + 1]),
            12 - (flags & WORD_LEN), "                               ",
            (flags & WORD_LEN),
            (flags & MASK_VIS) ? '+' : '-',//"VIS" : "INV",
            (flags & MASK_IMM) ? '+' : '-',//"IMM" : "NIM",
            end - addr - CELL_SIZE - 1 - (flags & WORD_LEN)
        );
        disasm(vm, addr, addr + CELL_SIZE + 1 + (flags & WORD_LEN), end);
    }
    puts("\n\n");
}

void stacks(VM *vm) {
    printf("IP<%i>\n", vm->ip);
    printf("P<%i> ", vm->psp);
    for(int i = 0; i < vm->psp; ++i)
        printf("%i ", vm->ps[i]);
    puts("");

    printf("R<%i> ", vm->rsp);
    for(int i = 0; i < vm->rsp; ++i)
        printf("%i ", vm->rs[i]);
    puts("");
}











