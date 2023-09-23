
#include "ff.h"
#include "ff_debug.h"

#include <stdio.h>


extern cell hp;
extern cell lp;

void disasm(VM *vm, cell addr, cell begin, cell end) {
    while(begin < end) {
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

    printf("NAME         LEN VIS IMM INSTR\n");
    printf("------------------------------\n");
    for(addr = lp, end = hp; addr != 0; end = addr, addr = *((cell *) &(vm->mem[addr]))) {
        flags = vm->mem[addr + CELL_SIZE];
        printf("%.*s %.*s%2i   %c   %c   %02i : ",
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

void stacks2(VM *vm) {
    for(int i = 0; i < vm->psp; ++i)
        printf("%i ", vm->ps[i]);
    printf(">>> ");
}
