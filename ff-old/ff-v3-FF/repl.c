#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef REPL_H_
#define REPL_H_
#include "repl.h"
#endif

#ifndef UTILS_H_
#define UTILS_H_
#include "utils.h"
#endif

#ifndef OPCODES_H_
#define OPCODES_H_
#include "opcodes.h"
#endif


void repl(VM *vm) {
	char buffer[MAX_LEN + 1];
	while(scanf("%31s", buffer) != EOF) {
        eval_word(vm, buffer);
#ifdef DEBUG_REPL
        debug(vm);
#endif
    }
}
void eval(VM *vm, char *str) {
	char *delimiter = " \t\n\v\f\r";
    char *word = strtok(str, delimiter);
    while(word != NULL) {
        eval_word(vm, word);
#ifdef DEBUG_REPL
        debug(vm);
#endif
        word = strtok(NULL, delimiter);
    }
}
void eval_word(VM *vm, char *word) {
    switch(vm->mode) {
    case INTERPRET: eval_intr(vm, word); break;
    case COMPILE: eval_comp(vm, word); break;
    default: return;
    }
    run_vm(vm);
}
void eval_comp(VM *vm, char *word){
    ptr i = 0;
    ptr addr = find_addr(vm, word);
    byte flags = vm->mem[addr + CELL_SIZE];
	if(addr && (flags & MASK_IMM)) {
        ptr num = addr + CELL_SIZE + 1 + (flags & MASK_LEN);
        vm->mem[i++] = OP_LIT;
        memcpy(vm->mem + i, &num, CELL_SIZE);
        i += CELL_SIZE;
        vm->mem[i++] = OP_CALL;
    } else if(addr && !(flags & MASK_IMM)) {
        ptr num = addr + CELL_SIZE + 1 + (flags & MASK_LEN);
        vm->mem[(vm->mem_ptr)++] = OP_LIT;
        memcpy(vm->mem + vm->mem_ptr, &num, CELL_SIZE);
        vm->mem_ptr += CELL_SIZE;
        vm->mem[(vm->mem_ptr)++] = OP_CALL;
    } else if(isnumber(word)) {
        cell num = atoi(word);
        vm->mem[(vm->mem_ptr)++] = OP_LIT;
        memcpy(vm->mem + vm->mem_ptr, &num, CELL_SIZE);
        vm->mem_ptr += CELL_SIZE;
    } else {
        vm->mode = INTERPRET;
		vm->error = UNKNOWN_WORD;
        vm->mem_ptr = vm->wrd_ptr;
        memcpy(&(vm->wrd_ptr), &(vm->mem[vm->wrd_ptr]), CELL_SIZE);
	}
    vm->mem[i++] = OP_END;
}
void eval_intr(VM *vm, char *word){
    ptr i = 0;
    ptr addr = find_addr(vm, word);
    byte flags = vm->mem[addr + CELL_SIZE];
    if(addr) {
        ptr num = addr + CELL_SIZE + 1 + (flags & MASK_LEN);
        vm->mem[i++] = OP_LIT;
        memcpy(vm->mem + i, &num, CELL_SIZE);
        i += CELL_SIZE;
        vm->mem[i++] = OP_CALL;
    } else if(isnumber(word)) {
        cell num = atoi(word);
        vm->mem[i++] = OP_LIT;
        memcpy(vm->mem + i, &num, CELL_SIZE);
        i += CELL_SIZE;
    } else {
		vm->error = UNKNOWN_WORD;
    }
    vm->mem[i++] = OP_END;
}

ptr find_addr(VM *vm, const char *word) {
    for(ptr addr = vm->wrd_ptr; addr != 0; memcpy(&addr, vm->mem + addr, CELL_SIZE)) {
        byte flags = vm->mem[addr + CELL_SIZE];
        if((flags & MASK_LEN) == strlen(word) && (flags & MASK_VIS))
            if(strncmp(word, vm->mem + addr + CELL_SIZE + 1, (flags & MASK_LEN)) == 0)
                return addr;
    }
    return 0;
}
int isnumber(const char *word) {
    int i = word[0] == '-' ? 1 : 0;
    for(;word[i] != '\0'; ++i)
        if(!isdigit(word[i]))
            return 0;
    return 1;
}


