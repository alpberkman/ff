

void _nop(VM *vm) {
    (void) vm;
}
void _lit(VM *vm) {
    cell val = *((cell *) &(vm->mem[vm->ip]));
    vm->ps[vm->psp++] = val;
    vm->ip += CELL_SIZE;
}
void _halt(VM *vm) {
    vm->p = OFF;
}
void _dup(VM *vm) {
    cell val = vm->ps[(byte) (vm->psp - 1)];
    vm->ps[vm->psp++] = val;
}
void _drop(VM *vm) {
    vm->psp -= 1;
}
void _swap(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = a;
    vm->ps[vm->psp++] = b;
}
void _push(VM *vm) {
    vm->rs[vm->rsp++] = vm->ps[--vm->psp];
}
void _pop(VM *vm) {
    vm->ps[vm->psp++] = vm->rs[--vm->rsp];
}
void _pickp(VM *vm) {
    byte n = vm->ps[--vm->psp] + 1;
    n = vm->psp - n;
    vm->ps[vm->psp++] = vm->ps[n];
}
void _pickr(VM *vm) {
    byte n = vm->ps[--vm->psp] + 1;
    n = vm->rsp - n;
    vm->ps[vm->psp++] = vm->rs[n];
}
void _jmp(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    vm->ip = addr;
}
void _jz(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    cell flag = vm->ps[--vm->psp];
    if(flag == 0)
        vm->ip = addr;
}
void _ret(VM *vm) {
    cell addr = vm->rs[--vm->rsp];
    vm->ip = addr;
}
void _eq(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b == a ? TRUE : FALSE;
}
void _neq(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b != a ? TRUE : FALSE;
}
void _lt(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b < a ? TRUE : FALSE;
}
void _gt(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b > a ? TRUE : FALSE;
}
void _and(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b & a;
}
void _or(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b | a;
}
void _xor(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b ^ a;
}
void _shr(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b >> a;
}
void _shl(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b << a;
}
void _add(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b + a;
}
void _sub(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b - a;
}
void _mul(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b * a;
}
void _div(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b / a;
}
void _mod(VM *vm) {
    cell a = vm->ps[--vm->psp];
    cell b = vm->ps[--vm->psp];
    vm->ps[vm->psp++] = b % a;
}
void _ldc(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    cell val = *((cell *) &(vm->mem[addr]));
    vm->ps[vm->psp++] = val;
}
void _strc(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    cell val = vm->ps[--vm->psp];
    *((cell *) &(vm->mem[addr])) = val;
}
void _ldb(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    byte val = vm->mem[addr];
    vm->ps[vm->psp++] = val;
}
void _strb(VM *vm) {
    cell addr = vm->ps[--vm->psp];
    byte val = vm->ps[--vm->psp];
    vm->mem[addr] = val;
}
void _lds(VM *vm) {
    cell val = vm->s;
    vm->ps[vm->psp++] = val;
}
void _strs(VM *vm) {
    cell val = vm->ps[--vm->psp];
    vm->s = val;
}
void _ldp(VM *vm) {
    byte val = vm->psp;
    vm->ps[vm->psp++] = val;
}
void _strp(VM *vm) {
    byte val = vm->ps[--vm->psp];
    vm->psp = val;
}
void _ldr(VM *vm) {
    byte val = vm->rsp;
    vm->ps[vm->psp++] = val;
}
void _strr(VM *vm) {
    byte val = vm->ps[--vm->psp];
    vm->rsp = val;
}
void _ldi(VM *vm) {
    cell val = vm->ip;
    vm->ps[vm->psp++] = val;
}
void _stri(VM *vm) {
    cell val = vm->ps[--vm->psp];
    vm->ip = val;
}
void _ldh(VM *vm) {
    cell val = vm->hp;
    vm->ps[vm->psp++] = val;
}
void _strh(VM *vm) {
    cell val = vm->ps[--vm->psp];
    vm->hp = val;
}
void _ldl(VM *vm) {
    cell val = vm->lp;
    vm->ps[vm->psp++] = val;
}
void _strl(VM *vm) {
    cell val = vm->ps[--vm->psp];
    vm->lp = val;
}
void _key(VM *vm) {
    int c = fgetc(vm->i);
    vm->ps[vm->psp++] = c;
}
void _emit(VM *vm) {
    int c = vm->ps[--vm->psp];
    fputc(c, vm->o);
}
void _csz(VM *vm) {
    vm->ps[vm->psp++] = CELL_SIZE;
}
void _cfun(VM *vm) {
    fun cfun = *((fun *) &(vm->mem[vm->ip]));
    vm->ip += CFUN_SIZE;
    cfun(vm);
}

