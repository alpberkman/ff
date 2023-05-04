

/*
 Pretty prining decoder
 */
void print_opcode(VM *vm, cell opcode) {
    switch(opcode) {
    case NOP:
        printf("PRIM-NOP ");
        break;
    case LIT:
        printf("PRIM-LIT ");
        break;
    case HALT:
        printf("PRIM-HALT ");
        break;
    case DUP:
        printf("PRIM-DUP ");
        break;
    case DROP:
        printf("PRIM-DROP ");
        break;
    case SWAP:
        printf("PRIM-SWAP ");
        break;
    case PUSH:
        printf("PRIM-PUSH ");
        break;
    case POP:
        printf("PRIM-POP ");
        break;
    case PICKP:
        printf("PRIM-PICKP ");
        break;
    case PICKR:
        printf("PRIM-PICKR ");
        break;
    case JMP:
        printf("PRIM-JMP ");
        break;
    case JZ:
        printf("PRIM-JZ ");
        break;
    case RET:
        printf("PRIM-RET ");
        break;
    case EQ:
        printf("PRIM-EQ ");
        break;
    case NEQ:
        printf("PRIM-NEQ ");
        break;
    case LT:
        printf("PRIM-LT ");
        break;
    case GT:
        printf("PRIM-GT ");
        break;
    case AND:
        printf("PRIM-AND ");
        break;
    case OR:
        printf("PRIM-OR ");
        break;
    case XOR:
        printf("PRIM-XOR ");
        break;
    case SHR:
        printf("PRIM-SHR ");
        break;
    case SHL:
        printf("PRIM-SHL ");
        break;
    case ADD:
        printf("PRIM-ADD ");
        break;
    case SUB:
        printf("PRIM-SUB ");
        break;
    case MUL:
        printf("PRIM-MUL ");
        break;
    case DIV:
        printf("PRIM-DIV ");
        break;
    case MOD:
        printf("PRIM-MOD ");
        break;
    case LDC:
        printf("PRIM-LDC ");
        break;
    case STRC:
        printf("PRIM-STRC ");
        break;
    case LDB:
        printf("PRIM-LDB ");
        break;
    case STRB:
        printf("PRIM-STRB ");
        break;
    case LDS:
        printf("PRIM-LDS ");
        break;
    case STRS:
        printf("PRIM-STRS ");
        break;
    case LDP:
        printf("PRIM-LDP ");
        break;
    case STRP:
        printf("PRIM-STRP ");
        break;
    case LDR:
        printf("PRIM-LDR ");
        break;
    case STRR:
        printf("PRIM-STRR ");
        break;
    case LDI:
        printf("PRIM-LDI ");
        break;
    case STRI:
        printf("PRIM-STRI ");
        break;
    case LDH:
        printf("PRIM-LDH ");
        break;
    case STRH:
        printf("PRIM-STRH ");
        break;
    case LDL:
        printf("PRIM-LDL ");
        break;
    case STRL:
        printf("PRIM-STRL ");
        break;
    case CSZ:
        printf("PRIM-CSZ ");
        break;
    case CFUN:
        printf("PRIM-CFUN ");
        break;
    case KEY:
        printf("PRIM-KEY ");
        break;
    case EMIT:
        printf("PRIM-EMIT ");
        break;
    default: {
        cell addr;
        for(addr = vm->lp; addr > opcode; addr = *((cell *) &(vm->mem[addr])));
        printf("%.*s ", vm->mem[addr+CELL_SIZE] & WORD_LEN, &(vm->mem[addr+CELL_SIZE+1]));
    }

    }
}
void print_word(VM *vm, cell addr, cell end) {

    printf("0x%04x: 0x%04x (%i%s%s) %.*s:  %ib\n", addr, vm->mem[addr],
           vm->mem[addr+CELL_SIZE] & WORD_LEN,
           vm->mem[addr+CELL_SIZE] & MASK_VIS ? " | VIS" : "",
           vm->mem[addr+CELL_SIZE] & MASK_IMM ? " | IMM" : "",
           vm->mem[addr+CELL_SIZE] & WORD_LEN, &(vm->mem[addr+CELL_SIZE+1]),
           end - addr);

    addr += CELL_SIZE + 1 + (vm->mem[addr+CELL_SIZE] & WORD_LEN);
    while(addr < end) {

        print_opcode(vm, *((cell *) &vm->mem[addr]));

        if(*((cell *) &vm->mem[addr]) == (cell)LIT) {
            addr += CELL_SIZE;
            printf("%i ", *((cell *) &vm->mem[addr]));
        } else if(*((cell *) &vm->mem[addr]) == CFUN) {
            addr += CFUN_SIZE;
        }

        addr += CELL_SIZE;
    }
    puts("\n");
}


/*
 General debugging functions
 */
void debug_state(VM *vm) {
    printf("Power: %s\n", vm->p == OFF ? "OFF" : "ON");
    printf("State: %s\n", vm->s == INTERPRET ? "INTERPRET" : "COMPILE");
    printf("IP: %i  HP: %i  LP: %i\n", vm->ip, vm->hp, vm->lp);
}
void debug_stack(VM *vm) {
    int i;
    printf("PS: %6i\tRS: %6i\n", vm->psp, vm->rsp);
    for(i = 0; i < (vm->psp > vm->rsp ? vm->psp : vm->rsp); ++i) {
        if(vm->psp > i)
            printf("%10i\t", vm->ps[i]);
        else
            printf("          \t");

        if(vm->rsp > i)
            printf("%10i\n", vm->rs[i]);
        else
            printf("          \n");
    }
}
void debug_stack2(VM *vm) {
    int i;

    printf("PS %6i: ", vm->psp);
    for(i = 0; i < vm->psp; ++i)
        printf("%6i ", vm->ps[i]);
    puts("");

    printf("RS %6i: ", vm->rsp);
    for(i = 0; i < vm->rsp; ++i)
        printf("%6i ", vm->rs[i]);
    puts("");
    puts("");
}
void debug_mem(VM *vm) {
    int i;
    for(i = 0; i < vm->hp; ++i) {
        printf("0x%04x: %3i %c\n", i, vm->mem[i], isgraph(vm->mem[i]) ? vm->mem[i] : '_');
        print_opcode(vm, vm->mem[i]);
    }
}
void debug_words(VM *vm) {
    cell addr;
    cell end;
    for(addr = vm->lp, end = vm->hp; addr != 0; end = addr, addr = *((cell *) &(vm->mem[addr])))
        print_word(vm,  addr, end);
}
void debug_words2(VM *vm) {
    cell addr;
    for(addr = vm->lp; addr != 0; addr = *((cell *) &(vm->mem[addr])))
        printf("%.*s ", vm->mem[addr+CELL_SIZE] & WORD_LEN, &(vm->mem[addr+CELL_SIZE+1]));
    puts("");
}
void debug_word(VM *vm) {
    char buf[32];
    int c;
    cell addr = 0;
    byte flags = 0;
    cell end = 0;

    /*Remove white space*/
    while(isspace(c = getchar()));

    /*Get the string*/
    byte len = 0;
    do {
        if(c == EOF)
            return;
        buf[len++] = toupper(c);
    } while(len < 31 && !isspace(c = getchar()));
    buf[len] = '\0';

    for(addr = vm->lp, end = vm->hp; addr != 0; end = addr, addr = *((cell *) &(vm->mem[addr]))) {
        flags = vm->mem[addr + CELL_SIZE];
        if((flags & MASK_VIS) && len == (flags & WORD_LEN))
            if(strncmp(buf, (char *) &(vm->mem[addr + CELL_SIZE + 1]), len) == 0)
                break;
    }

    if(addr == 0)
        printf("Unknown word\n");
    else
        print_word(vm, addr, end);
}
void debug(VM *vm) {
    printf("\nDebug Info\n");
    debug_state(vm);
    debug_stack(vm);
    puts("");
    debug_words(vm);
    puts("");
    puts("");
}


/*
 Tools for saving
 */
void save(VM *vm, char *file) {
    int i;
    FILE *fp = fopen(file, "wb");
    for(i = 0; i < vm->hp; ++i)
        fputc(vm->mem[i], fp);
    fclose(fp);
}
void restore(VM *vm, char *file) {
    int c;
    FILE *fp = fopen(file, "rb");
    while((c = fgetc(fp)) != EOF)
        vm->mem[vm->hp++] = c;
    fclose(fp);
}
void dump(VM *vm, char *file) {
    int i;
    FILE *fp = fopen(file, "w");
    fprintf(fp, "{ ");
    for(i = 0; i < vm->hp; ++i) {
        if(i % 8 == 0)
            fprintf(fp, "\n");
        fprintf(fp, "%#05x, ", vm->mem[i]);
    }
    fprintf(fp, "\n};");

}



