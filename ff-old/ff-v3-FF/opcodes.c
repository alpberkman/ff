#include <string.h>

#ifndef OPCODES_H_
#define OPCODES_H_
#include "opcodes.h"
#endif


void eval_opcode(VM *vm, byte b) {
    switch(b) {
    case OP_NOP: eval_nop(vm); break;
    case OP_LIT: eval_lit(vm); break;
    case OP_END: eval_end(vm); break;
    case OP_DUP: eval_dup(vm); break;
    case OP_DROP: eval_drop(vm); break;
    case OP_SWAP: eval_swap(vm); break;
    case OP_PUSH: eval_push(vm); break;
    case OP_POP: eval_pop(vm); break;
    case OP_JMP: eval_jmp(vm); break;
    case OP_JZ: eval_jz(vm); break;
    case OP_CALL: eval_call(vm); break;
    case OP_RET: eval_ret(vm); break;
    case OP_EQ: eval_eq(vm); break;
    case OP_NEQ: eval_neq(vm); break;
    case OP_LT: eval_lt(vm); break;
    case OP_GT: eval_gt(vm); break;
    case OP_NOT: eval_not(vm); break;
    case OP_AND: eval_and(vm); break;
    case OP_OR: eval_or(vm); break;
    case OP_XOR: eval_xor(vm); break;
    case OP_INV: eval_inv(vm); break;
    case OP_SHR: eval_shr(vm); break;
    case OP_SHL: eval_shl(vm); break;
    case OP_ADD: eval_add(vm); break;
    case OP_SUB: eval_sub(vm); break;
    case OP_MUL: eval_mul(vm); break;
    case OP_DIV: eval_div(vm); break;
    case OP_MOD: eval_mod(vm); break;
    case OP_FETCH: eval_fetch(vm); break;
    case OP_STORE: eval_store(vm); break;
    case OP_QUERY: eval_query(vm); break;
    case OP_UPDATE: eval_update(vm); break;
    case OP_FUN: eval_fun(vm); break;
    default: vm->error = UNKNOWN_OPCODE;
    }
}


void eval_nop(VM *vm){
}
void eval_lit(VM *vm){
	if(vm->i_ptr + CELL_SIZE > vm->mem_ptr)
		return;
	memcpy(&(vm->reg_a), vm->mem + vm->i_ptr, CELL_SIZE);
	vm->i_ptr += CELL_SIZE;
	ppush(vm, vm->reg_a);
}
void eval_end(VM *vm){
    vm->status = DEAD;
}

void eval_dup(VM *vm){
	vm->reg_a = ppop(vm);
    ppush(vm, vm->reg_a);
    ppush(vm, vm->reg_a);
}
void eval_drop(VM *vm){
    ppop(vm);
}
void eval_swap(VM *vm){
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_a);
    ppush(vm, vm->reg_b);
}
void eval_push(VM *vm){
	vm->reg_a = ppop(vm);
    rpush(vm, vm->reg_a);
}
void eval_pop(VM *vm){
	vm->reg_a = rpop(vm);
    ppush(vm, vm->reg_a);
}

void eval_jmp(VM *vm){
	vm->reg_a = ppop(vm);
    vm->i_ptr = vm->reg_a % MEMORY_SIZE;
}
void eval_jz(VM *vm){
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    if (vm->reg_b == 0)
        vm->i_ptr = vm->reg_a % MEMORY_SIZE;
}

void eval_call(VM *vm){
	vm->reg_a = ppop(vm);
    rpush(vm, vm->i_ptr);
    vm->i_ptr = vm->reg_a % MEMORY_SIZE;
}
void eval_ret(VM *vm){
	vm->reg_a = rpop(vm);
    vm->i_ptr = vm->reg_a % MEMORY_SIZE;
}

void eval_eq(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b == vm->reg_a ? TRUE : FALSE);
}
void eval_neq(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b != vm->reg_a ? TRUE : FALSE);
}
void eval_lt(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b < vm->reg_a ? TRUE : FALSE);
}
void eval_gt(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b > vm->reg_a ? TRUE : FALSE);
}

void eval_not(VM *vm) {
    vm->reg_a = ppop(vm);
    ppush(vm, !(vm->reg_a));
}
void eval_and(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b & vm->reg_a);
}
void eval_or(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b | vm->reg_a);
}
void eval_xor(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b ^ vm->reg_a);
}
void eval_inv(VM *vm) {
    vm->reg_a = ppop(vm);
    ppush(vm, ~(vm->reg_a));
}
void eval_shr(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b >> vm->reg_a);
}
void eval_shl(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b << vm->reg_a);
}

void eval_add(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b + vm->reg_a);
}
void eval_sub(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b - vm->reg_a);
}
void eval_mul(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b * vm->reg_a);
}
void eval_div(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b / vm->reg_a);
}
void eval_mod(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    ppush(vm, vm->reg_b % vm->reg_a);
}

void eval_fetch(VM *vm) {
    vm->reg_a = ppop(vm);
	vm->reg_a = get_mem(vm, vm->reg_a);
	ppush(vm, vm->reg_a);
}
void eval_store(VM *vm) {
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
	set_mem(vm, vm->reg_a, vm->reg_b);
}
void eval_query(VM *vm) {
    vm->reg_a = ppop(vm);
    switch (vm->reg_a) {
    case 1: ppush(vm, vm->mode); break;
    case 2: ppush(vm, vm->error); break;
    case 3: ppush(vm, vm->ps_ptr); break;
    case 4: ppush(vm, vm->rs_ptr); break;
    case 5: ppush(vm, vm->mem_ptr); break;
    case 6: ppush(vm, vm->wrd_ptr); break;
	default: ppush(vm, MEMORY_SIZE);
	}
}
void eval_update(VM *vm){
    vm->reg_a = ppop(vm);
    vm->reg_b = ppop(vm);
    switch (vm->reg_a) {
    case 1: vm->mode = vm->reg_b; break;
    case 2: vm->error = vm->reg_b; break;
    case 3: vm->ps_ptr = vm->reg_b; break;
    case 4: vm->rs_ptr = vm->reg_b; break;
    case 5: vm->mem_ptr = vm->reg_b; break;
    case 6: vm->wrd_ptr = vm->reg_b; break;
	}
}

void eval_fun(VM *vm){
	if(vm->i_ptr + VM_FUN_SIZE > vm->mem_ptr)
		return;
	vm_fun fun;
	memcpy(&fun, vm->mem + vm->i_ptr, VM_FUN_SIZE);
	vm->i_ptr += VM_FUN_SIZE;
	fun(vm);
}






















