
#ifndef VM_H_
#define VM_H_
#include "vm.h"
#endif


typedef enum OPCODE OPCODE;
enum OPCODE {

    OP_NOP,
    OP_LIT,
    OP_END,

    /*
     * Stack operations
     */
    OP_DUP,
    OP_DROP,
    OP_SWAP,
    OP_PUSH,
    OP_POP,

    /*
     * Jumps
     */
    OP_JMP,
    OP_JZ,

    /*
     * Function calls
     */
    OP_CALL,
    OP_RET,

    /*
     * Relational operations
     */
    OP_EQ,
    OP_NEQ,
    OP_LT,
    OP_GT,

    /*
     * Bitwise logical operations
     */
    OP_NOT,
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_INV,
    OP_SHR,
    OP_SHL,

    /*
     * Arithmetic operations
     */
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,

    /*
     * Memory operations
     */
    OP_FETCH,
    OP_STORE,
    OP_QUERY,
    OP_UPDATE,

    /*
     * Other operations
     */
    OP_FUN,

};


void eval_opcode(VM *vm, byte b);


void eval_nop(VM *vm);
void eval_lit(VM *vm);
void eval_end(VM *vm);

void eval_dup(VM *vm);
void eval_drop(VM *vm);
void eval_swap(VM *vm);
void eval_push(VM *vm);
void eval_pop(VM *vm);

void eval_jmp(VM *vm);
void eval_jz(VM *vm);

void eval_call(VM *vm);
void eval_ret(VM *vm);

void eval_eq(VM *vm);
void eval_neq(VM *vm);
void eval_lt(VM *vm);
void eval_gt(VM *vm);

void eval_not(VM *vm);
void eval_and(VM *vm);
void eval_or(VM *vm);
void eval_xor(VM *vm);
void eval_inv(VM *vm);
void eval_shr(VM *vm);
void eval_shl(VM *vm);

void eval_add(VM *vm);
void eval_sub(VM *vm);
void eval_mul(VM *vm);
void eval_div(VM *vm);
void eval_mod(VM *vm);

void eval_fetch(VM *vm);
void eval_store(VM *vm);
void eval_query(VM *vm);
void eval_update(VM *vm);

void eval_fun(VM *vm);


