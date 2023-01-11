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


int fget_input(char *buffer) {
    int len = 0;
    int c;

    while(!isspace((c = fgetc(stdin)))) {
		if(c == EOF)
			return EOF;
        buffer[len++] = c;
        if(len == MAX_LEN)
            break;
    }

    buffer[len] = '\0';
    return len;
}
void repl(VM *vm) {
    char buffer[MAX_LEN + 1];
	int c;
    while((c = fget_input(buffer)) != EOF) {
		if(c != 0) {
        	eval_word(vm, buffer);
#ifdef DEBUG_REPL
        	debug(vm);
#endif
		}
	}
}

char *script;
int sget_input(char *buffer) {
    int len = 0;
    int c;

    while(!isspace((c = *script++))) {
		if(c == '\0') {
			script = NULL;
			break;
		}
        buffer[len++] = c;
        if(len == MAX_LEN)
            break;
    }

    buffer[len] = '\0';
    return len;
}
void eval(VM *vm, char *str) {
	script = str;
	char buffer[MAX_LEN + 1];
	int c;
    while(script != NULL) {
		c = sget_input(buffer);
		if(c != 0) {
        	eval_word(vm, buffer);
#ifdef DEBUG_REPL
        	debug(vm);
#endif
		}
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


