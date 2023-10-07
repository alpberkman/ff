#include <string.h>
#include <stdio.h>

#ifndef UTILS_H_
#define UTILS_H_
#include "utils.h"
#endif

#ifndef OPCODES_H_
#define OPCODES_H_
#include "opcodes.h"
#endif


void init_vm(VM *vm) {

	vm->status = DEAD;
    vm->i_ptr = 0;

    vm->mode = INTERPRET;
    vm->error = NO_ERROR;

	memset(vm->ps, 0, PSTACK_SIZE*CELL_SIZE);
    vm->ps_ptr = 0;

	memset(vm->rs, 0, RSTACK_SIZE*CELL_SIZE);
    vm->rs_ptr = 0;

	memset(vm->mem, 0, MEMORY_SIZE);
    vm->mem_ptr = 8;
    vm->wrd_ptr = 0;
}
void save_vm(VM *vm, const char *image) {}
void load_vm(VM *vm, const char *image) {}

void make_app_from_img(const char *image) {}
void make_app_from_src(const char *source) {}

void extend_dict(VM *vm, char *name, byte flags, byte *fun, ptr len) {
    memcpy(vm->mem + vm->mem_ptr, &(vm->wrd_ptr), CELL_SIZE);
    vm->wrd_ptr = vm->mem_ptr;
    vm->mem_ptr += CELL_SIZE;

	vm->mem[(vm->mem_ptr)++] = flags | strlen(name);

    memcpy(vm->mem + vm->mem_ptr, name, strlen(name));
    vm->mem_ptr += strlen(name);

    memcpy(vm->mem + vm->mem_ptr, fun, len);
    vm->mem_ptr += len;

	vm->mem[(vm->mem_ptr)++] = OP_RET;
}
void extend_cffi(VM *vm, char *name, byte flags, vm_fun fun) {
    memcpy(vm->mem + vm->mem_ptr, &(vm->wrd_ptr), CELL_SIZE);
    vm->wrd_ptr = vm->mem_ptr;
    vm->mem_ptr += CELL_SIZE;

	vm->mem[(vm->mem_ptr)++] = flags | strlen(name);

    memcpy(vm->mem + vm->mem_ptr, name, strlen(name));
    vm->mem_ptr += strlen(name);

	vm->mem[(vm->mem_ptr)++] = OP_FUN;
	memcpy(vm->mem + vm->mem_ptr, &fun, VM_FUN_SIZE);
	vm->mem_ptr += VM_FUN_SIZE;

	vm->mem[(vm->mem_ptr)++] = OP_RET;
}

void debug(VM *vm) {
    printf("--DEBUG START--\n");

#ifdef DEBUG_MEM
    for(cell i = 0; i < vm->mem_ptr; ++i)
        printf("%#010x\t%i\n", i, vm->mem[i]);
#endif
    printf("Memory: %i\n", vm->mem_ptr);
    printf("Word: %i\n", vm->wrd_ptr);

    printf("Mode: %i\n", vm->mode);
    printf("Error: %i\n", vm->error);

    printf("Parameter stack: %i\n", vm->ps_ptr);
    for(cell i = 0; i < vm->ps_ptr; ++i)
        printf("%#010x\t%hi\n", i,  vm->ps[i]);

    printf("Return stack: %i\n", vm->rs_ptr);
    for(cell i = 0; i < vm->rs_ptr; ++i)
        printf("%#010x\t%hi\n", i, vm->rs[i]);

    printf("--DEBUG END--\n");
}


void hello_world(VM *vm) {
	printf("Hello world\n");
}
void key(VM *vm) {
	ppush(vm, getchar());
}
void emit(VM *vm) {
	putchar(ppop(vm));
}
void col(VM *vm) {
	char name[MAX_LEN + 1];
	scanf("%31s", name);

    memcpy(vm->mem + vm->mem_ptr, &(vm->wrd_ptr), CELL_SIZE);
    vm->wrd_ptr = vm->mem_ptr;
    vm->mem_ptr += CELL_SIZE;

	vm->mem[(vm->mem_ptr)++] = 0 | strlen(name);

    memcpy(vm->mem + vm->mem_ptr, name, strlen(name));
    vm->mem_ptr += strlen(name);

	vm->mode = COMPILE;
}
void semicol(VM *vm) {
	vm->mem[(vm->mem_ptr)++] = OP_RET;
	vm->mem[(vm->wrd_ptr) + CELL_SIZE] |= MASK_VIS;

	vm->mode = INTERPRET;
}
void extend(VM *vm) {

    byte nop[] = {OP_NOP};
	// OP_LIT
	byte end[] = {OP_END};
	extend_dict(vm, "NOP", MASK_VIS, nop, sizeof(nop));
	extend_dict(vm, "END", MASK_VIS, end, sizeof(end));

    byte dup[] = {OP_DUP};
    byte drop[] = {OP_DROP};
    byte swap[] = {OP_SWAP};
    byte push[] = {OP_POP, OP_SWAP, OP_PUSH, OP_PUSH};
    byte pop[] = {OP_POP, OP_POP, OP_SWAP, OP_PUSH};
	extend_dict(vm, "DUP", MASK_VIS, dup, sizeof(dup));
	extend_dict(vm, "DROP", MASK_VIS, drop, sizeof(drop));
	extend_dict(vm, "SWAP", MASK_VIS, swap, sizeof(swap));
	extend_dict(vm, "PUSH", MASK_VIS, push, sizeof(push));
	extend_dict(vm, "POP", MASK_VIS, pop, sizeof(pop));

    // OP_JMP,
    // OP_JZ,

    // OP_CALL,
    // OP_RET,

    byte eq[] = {OP_EQ};
    byte neq[] = {OP_NEQ};
    byte lt[] = {OP_LT};
    byte gt[] = {OP_GT};
	extend_dict(vm, "EQ", MASK_VIS, eq, sizeof(eq));
	extend_dict(vm, "NEQ", MASK_VIS, neq, sizeof(neq));
	extend_dict(vm, "LT", MASK_VIS, lt, sizeof(lt));
	extend_dict(vm, "GT", MASK_VIS, gt, sizeof(gt));

    byte not[] = {OP_NOT};
    byte and[] = {OP_AND};
    byte or[] = {OP_OR};
    byte xor[] = {OP_XOR};
    byte inv[] = {OP_INV};
    byte shr[] = {OP_SHR};
    byte shl[] = {OP_SHL};
	extend_dict(vm, "NOT", MASK_VIS, not, sizeof(not));
	extend_dict(vm, "AND", MASK_VIS, and, sizeof(and));
	extend_dict(vm, "OR", MASK_VIS, or, sizeof(or));
	extend_dict(vm, "XOR", MASK_VIS, xor, sizeof(xor));
	extend_dict(vm, "INV", MASK_VIS, inv, sizeof(inv));
	extend_dict(vm, "SHR", MASK_VIS, shr, sizeof(shr));
	extend_dict(vm, "SHL", MASK_VIS, shl, sizeof(shl));

    byte add[] = {OP_ADD};
    byte sub[] = {OP_SUB};
    byte mul[] = {OP_MUL};
    byte div[] = {OP_DIV};
    byte mod[] = {OP_MOD};
	extend_dict(vm, "ADD", MASK_VIS, add, sizeof(add));
	extend_dict(vm, "SUB", MASK_VIS, sub, sizeof(sub));
	extend_dict(vm, "MUL", MASK_VIS, mul, sizeof(mul));
	extend_dict(vm, "DIV", MASK_VIS, div, sizeof(div));
	extend_dict(vm, "MOD", MASK_VIS, mod, sizeof(mod));

    byte fetch[] = {OP_FETCH};
    byte store[] = {OP_STORE};
    byte query[] = {OP_QUERY};
    byte update[] = {OP_UPDATE};
	extend_dict(vm, "FETCH", MASK_VIS, fetch, sizeof(fetch));
	extend_dict(vm, "STORE", MASK_VIS, store, sizeof(store));
	extend_dict(vm, "QUERY", MASK_VIS, query, sizeof(query));
	extend_dict(vm, "UPDATE", MASK_VIS, update, sizeof(update));

	extend_cffi(vm, "DEBUG", MASK_VIS | MASK_IMM, debug);
	extend_cffi(vm, "HELLO-WORLD", MASK_VIS, hello_world);
	extend_cffi(vm, "KEY", MASK_VIS, key);
	extend_cffi(vm, "EMIT", MASK_VIS, emit);
	extend_cffi(vm, ":", MASK_VIS, col);
	extend_cffi(vm, ";", MASK_VIS|MASK_IMM, semicol);
}


