

cell fetch(VM *vm) {
    cell opcode = *((cell *) &(vm->mem[vm->ip]));
    vm->ip += CELL_SIZE;
    return opcode;
}
void exec(VM *vm, cell opcode) {
    switch(opcode) {
    case NOP:
        _nop(vm);
        break;
    case LIT:
        _lit(vm);
        break;
    case HALT:
        _halt(vm);
        break;
    case DUP:
        _dup(vm);
        break;
    case DROP:
        _drop(vm);
        break;
    case SWAP:
        _swap(vm);
        break;
    case PUSH:
        _push(vm);
        break;
    case POP:
        _pop(vm);
        break;
    case PICKP:
        _pickp(vm);
        break;
    case PICKR:
        _pickr(vm);
        break;
    case JMP:
        _jmp(vm);
        break;
    case JZ:
        _jz(vm);
        break;
    case RET:
        _ret(vm);
        break;
    case EQ:
        _eq(vm);
        break;
    case NEQ:
        _neq(vm);
        break;
    case LT:
        _lt(vm);
        break;
    case GT:
        _gt(vm);
        break;
    case AND:
        _and(vm);
        break;
    case OR:
        _or(vm);
        break;
    case XOR:
        _xor(vm);
        break;
    case SHR:
        _shr(vm);
        break;
    case SHL:
        _shl(vm);
        break;
    case ADD:
        _add(vm);
        break;
    case SUB:
        _sub(vm);
        break;
    case MUL:
        _mul(vm);
        break;
    case DIV:
        _div(vm);
        break;
    case MOD:
        _mod(vm);
        break;
    case LDC:
        _ldc(vm);
        break;
    case STRC:
        _strc(vm);
        break;
    case LDB:
        _ldb(vm);
        break;
    case STRB:
        _strb(vm);
        break;
    case LDS:
        _lds(vm);
        break;
    case STRS:
        _strs(vm);
        break;
    case LDP:
        _ldp(vm);
        break;
    case STRP:
        _strp(vm);
        break;
    case LDR:
        _ldr(vm);
        break;
    case STRR:
        _strr(vm);
        break;
    case LDI:
        _ldi(vm);
        break;
    case STRI:
        _stri(vm);
        break;
    case LDH:
        _ldh(vm);
        break;
    case STRH:
        _strh(vm);
        break;
    case LDL:
        _ldl(vm);
        break;
    case STRL:
        _strl(vm);
        break;
    case CSZ:
        _csz(vm);
        break;
    case CFUN:
        _cfun(vm);
        break;
    case KEY:
        _key(vm);
        break;
    case EMIT:
        _emit(vm);
        break;
    default:
        vm->rs[vm->rsp++] = vm->ip;
        vm->ip = opcode;
    }
}
void cycle(VM *vm) {
    exec(vm, fetch(vm));
}
void run(VM *vm) {
    while(vm->p == ON)
        cycle(vm);
}

