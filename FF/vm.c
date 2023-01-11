
#ifndef OPCODES_H_
#define OPCODES_H_
#include "opcodes.h"
#endif


VM_ERROR run_vm(VM *vm) {
	vm->status = ALIVE;
    vm->i_ptr = 0;
    while(vm->status)
		eval_opcode(vm, get_opcode(vm));
	return vm->error;
}
byte get_opcode(VM *vm) {
	if(vm->i_ptr >= MEMORY_SIZE) {
		vm->error = END_OF_MEM;
        return OP_END;
	}
    return vm->mem[(vm->i_ptr)++];
}

byte get_mem(VM *vm, cell addr) {
    return vm->mem[addr%MEMORY_SIZE];
}
void set_mem(VM *vm, cell addr, cell c) {
	vm->mem[addr%MEMORY_SIZE] = c;
}

void ppush(VM *vm, cell c) {
	if(vm->ps_ptr >= PSTACK_SIZE) {
		vm->error = PS_OVERFLOW;
		return;
	}
    vm->ps[(vm->ps_ptr)++] = c;
}
cell ppop(VM *vm){
	if(vm->ps_ptr <= 0) {
		vm->error = PS_UNDERFLOW;
		return 0;
	}
    return vm->ps[--(vm->ps_ptr)];
}
void rpush(VM *vm, cell c){
	if(vm->rs_ptr >= RSTACK_SIZE) {
		vm->error = RS_OVERFLOW;
		return;
	}
    vm->rs[(vm->rs_ptr)++] = c;
}
cell rpop(VM *vm){
	if(vm->rs_ptr < 0) {
		vm->error = RS_UNDERFLOW;
		return 0;
	}
    return vm->rs[--(vm->rs_ptr)];
}















