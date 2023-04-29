

void init(VM *vm) {

    int i;

    vm->p = ON;
    vm->s = INTERPRET;
    vm->i = stdin;
    vm->o = stdout;

    for(i = 0; i < 0x100; ++i)
        vm->ps[i] = 0;
    vm->psp = 0;

    for(i = 0; i < 0x100; ++i)
        vm->rs[i] = 0;
    vm->rsp = 0;

    for(i = 0; i < MEM_SIZE; ++i)
        vm->mem[i] = 0;

    vm->ip = 0;
    vm->hp = 0;
    vm->lp = 0;

    *((cell *) &(vm->mem[vm->hp])) = NOP;
    vm->hp += CELL_SIZE;
    *((cell *) &(vm->mem[vm->hp])) = HALT;
    vm->hp += CELL_SIZE;
}

cell word(VM *vm, char *name, char *fun, int len, char flag) {

    cell i;
    cell addr;

    *((cell *) &(vm->mem[vm->hp])) = *((cell *) &(vm->lp));
    vm->lp = vm->hp;
    vm->hp += CELL_SIZE;

    vm->mem[vm->hp++] = strlen(name) | flag;

    for(i = 0; i < (cell) strlen(name); ++i)
        vm->mem[vm->hp + i] = toupper(name[i]);
    vm->hp += strlen(name);

    addr = vm->hp;
    for(i = 0; i < len; ++i)
        vm->mem[vm->hp + i] = fun[i];
    vm->hp += len;

    return addr;
}

cell cword(VM *vm, char *name, fun cfun, char flag) {

    cell i;
    cell addr;

    *((cell *) &(vm->mem[vm->hp])) = *((cell *) &(vm->lp));
    vm->lp = vm->hp;
    vm->hp += CELL_SIZE;

    vm->mem[vm->hp++] = strlen(name) | flag;

    for(i = 0; i < (cell) strlen(name); ++i)
        vm->mem[vm->hp + i] = toupper(name[i]);
    vm->hp += strlen(name);

    addr = vm->hp;
    *((cell *) &(vm->mem[vm->hp])) = CFUN;
    vm->hp += CELL_SIZE;
    *((fun *) &(vm->mem[vm->hp])) = cfun;
    vm->hp += CFUN_SIZE;
    *((cell *) &(vm->mem[vm->hp])) = RET;
    vm->hp += CELL_SIZE;

    return addr;
}

void words(VM *vm) {
    cell addr = NOP;

    cell arr_nop[] = {
        RET,
    };
    cell arr_lit[] = {
        POP, DUP, CSZ, ADD, PUSH, LDC, RET,
    };
    cell arr_halt[] = {
        HALT,
    };
    cell arr_dup[] = {
        DUP, RET,
    };
    cell arr_drop[] = {
        DROP, RET,
    };
    cell arr_swap[] = {
        SWAP, RET,
    };
    cell arr_push[] = {
        POP, SWAP, PUSH, PUSH, RET,
    };
    cell arr_pop[] = {
        POP, POP, SWAP, PUSH, RET,
    };
    cell arr_pickp[] = {
        PICKP, RET,
    };
    cell arr_pickr[] = {
        LIT, 1, ADD, PICKR, RET,
    };
    cell arr_jmp[] = {
        POP, DROP, JMP,
    };
    cell arr_jz[] = {
        SWAP, PUSH, CSZ, LIT, 7, MUL, LDI, ADD, SWAP, JZ,
        POP, DROP, POP, JMP,
        POP, POP, DROP, JMP,
        /*Wrong*/
    };
    cell arr_ret[] = {
        POP, DROP, RET,
    };
    cell arr_eq[] = {
        EQ, RET,
    };
    cell arr_neq[] = {
        NEQ, RET,
    };
    cell arr_lt[] = {
        LT, RET,
    };
    cell arr_gt[] = {
        GT, RET,
    };
    cell arr_and[] = {
        AND, RET,
    };
    cell arr_or[] = {
        OR, RET,
    };
    cell arr_xor[] = {
        XOR, RET,
    };
    cell arr_shr[] = {
        SHR, RET,
    };
    cell arr_shl[] = {
        SHL, RET,
    };
    cell arr_add[] = {
        ADD, RET,
    };
    cell arr_sub[] = {
        SUB, RET,
    };
    cell arr_mul[] = {
        MUL, RET,
    };
    cell arr_div[] = {
        DIV, RET,
    };
    cell arr_mod[] = {
        MOD, RET,
    };
    cell arr_ldc[] = {
        LDC, RET,
    };
    cell arr_strc[] = {
        STRC, RET,
    };
    cell arr_ldb[] = {
        LDB, RET,
    };
    cell arr_strb[] = {
        STRB, RET,
    };
    cell arr_lds[] = {
        LDS, RET,
    };
    cell arr_strs[] = {
        STRS, RET,
    };
    cell arr_ldp[] = {
        LDP, RET,
    };
    cell arr_strp[] = {
        STRP, RET,
    };
    cell arr_ldr[] = {
        LDR, RET,
    };
    cell arr_strr[] = {
        STRR, RET,
    };
    cell arr_ldh[] = {
        LDH, RET,
    };
    cell arr_strh[] = {
        STRH, RET,
    };
    cell arr_ldl[] = {
        LDL, RET,
    };
    cell arr_strl[] = {
        STRL, RET,
    };
    cell arr_csz[] = {
        CSZ, RET,
    };
    cell arr_cfun[] = {
        CFUN, RET,
    };
    cell arr_key[] = {
        KEY, RET,
    };
    cell arr_emit[] = {
        EMIT, RET,
    };

    word(vm, "NOP", (char *) arr_nop, sizeof(arr_nop), MASK_VIS);
    word(vm, "LIT", (char *) arr_lit, sizeof(arr_lit), MASK_VIS);
    word(vm, "HALT", (char *) arr_halt, sizeof(arr_halt), MASK_VIS);
    word(vm, "DUP", (char *) arr_dup, sizeof(arr_dup), MASK_VIS);
    word(vm, "DROP", (char *) arr_drop, sizeof(arr_drop), MASK_VIS);
    word(vm, "SWAP", (char *) arr_swap, sizeof(arr_swap), MASK_VIS);
    word(vm, "PUSH", (char *) arr_push, sizeof(arr_push), MASK_VIS);
    word(vm, "POP", (char *) arr_pop, sizeof(arr_pop), MASK_VIS);
    word(vm, "PICKP", (char *) arr_pickp, sizeof(arr_pickp), MASK_VIS);
    word(vm, "PICKR", (char *) arr_pickr, sizeof(arr_pickr), MASK_VIS);
    word(vm, "JMP", (char *) arr_jmp, sizeof(arr_jmp), MASK_VIS);
    word(vm, "JZ", (char *) arr_jz, sizeof(arr_jz), MASK_VIS);
    word(vm, "RET", (char *) arr_ret, sizeof(arr_ret), MASK_VIS);
    word(vm, "EQ", (char *) arr_eq, sizeof(arr_eq), MASK_VIS);
    word(vm, "NEQ", (char *) arr_neq, sizeof(arr_neq), MASK_VIS);
    word(vm, "LT", (char *) arr_lt, sizeof(arr_lt), MASK_VIS);
    word(vm, "GT", (char *) arr_gt, sizeof(arr_gt), MASK_VIS);
    word(vm, "AND", (char *) arr_and, sizeof(arr_and), MASK_VIS);
    word(vm, "OR", (char *) arr_or, sizeof(arr_or), MASK_VIS);
    word(vm, "XOR", (char *) arr_xor, sizeof(arr_xor), MASK_VIS);
    word(vm, "SHR", (char *) arr_shr, sizeof(arr_shr), MASK_VIS);
    word(vm, "SHL", (char *) arr_shl, sizeof(arr_shl), MASK_VIS);
    word(vm, "ADD", (char *) arr_add, sizeof(arr_add), MASK_VIS);
    word(vm, "SUB", (char *) arr_sub, sizeof(arr_sub), MASK_VIS);
    word(vm, "MUL", (char *) arr_mul, sizeof(arr_mul), MASK_VIS);
    word(vm, "DIV", (char *) arr_div, sizeof(arr_div), MASK_VIS);
    word(vm, "MOD", (char *) arr_mod, sizeof(arr_mod), MASK_VIS);
    word(vm, "LDC", (char *) arr_ldc, sizeof(arr_ldc), MASK_VIS);
    word(vm, "STRC", (char *) arr_strc, sizeof(arr_strc), MASK_VIS);
    word(vm, "LDB", (char *) arr_ldb, sizeof(arr_ldb), MASK_VIS);
    word(vm, "STRB", (char *) arr_strb, sizeof(arr_strb), MASK_VIS);
    word(vm, "LDS", (char *) arr_lds, sizeof(arr_lds), MASK_VIS);
    word(vm, "STRS", (char *) arr_strs, sizeof(arr_strs), MASK_VIS);
    word(vm, "LDP", (char *) arr_ldp, sizeof(arr_ldp), MASK_VIS);
    word(vm, "STRP", (char *) arr_strp, sizeof(arr_strp), MASK_VIS);
    word(vm, "LDR", (char *) arr_ldr, sizeof(arr_ldr), MASK_VIS);
    word(vm, "STRR", (char *) arr_strr, sizeof(arr_strr), MASK_VIS);
    word(vm, "LDH", (char *) arr_ldh, sizeof(arr_ldh), MASK_VIS);
    word(vm, "STRH", (char *) arr_strh, sizeof(arr_strh), MASK_VIS);
    word(vm, "LDL", (char *) arr_ldl, sizeof(arr_ldl), MASK_VIS);
    word(vm, "STRL", (char *) arr_strl, sizeof(arr_strl), MASK_VIS);
    word(vm, "CSZ", (char *) arr_csz, sizeof(arr_csz), MASK_VIS);
    word(vm, "CFUN", (char *) arr_cfun, sizeof(arr_cfun), MASK_VIS);
    word(vm, "KEY", (char *) arr_key, sizeof(arr_key), MASK_VIS);
    word(vm, "EMIT", (char *) arr_emit, sizeof(arr_emit), MASK_VIS);

    cword(vm, "deb", debug, MASK_VIS | MASK_IMM);
    cword(vm, "deb_state", debug_state, MASK_VIS | MASK_IMM);
    cword(vm, "deb_stack", debug_stack, MASK_VIS | MASK_IMM);
    cword(vm, "deb_words", debug_words, MASK_VIS | MASK_IMM);
    cword(vm, "deb_mem", debug_mem, MASK_VIS | MASK_IMM);
    cword(vm, "deb_word", debug_word, MASK_VIS | MASK_IMM);
    cword(vm, "dw", debug_word, MASK_VIS | MASK_IMM);


    *((cell *) &(vm->mem[0])) = addr;
}

