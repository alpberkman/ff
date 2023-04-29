

/*
 Pretty prining decoder
 */
void print_opcode(VM *vm, cell opcode) {
    switch(opcode) {
    case NOP:
        printf("NOP ");
        break;
    case LIT:
        printf("LIT ");
        break;
    case HALT:
        printf("HALT ");
        break;
    case DUP:
        printf("DUP ");
        break;
    case DROP:
        printf("DROP ");
        break;
    case SWAP:
        printf("SWAP ");
        break;
    case PUSH:
        printf("PUSH ");
        break;
    case POP:
        printf("POP ");
        break;
    case PICKP:
        printf("PICKP ");
        break;
    case PICKR:
        printf("PICKR ");
        break;
    case JMP:
        printf("JMP ");
        break;
    case JZ:
        printf("JZ ");
        break;
    case RET:
        printf("RET ");
        break;
    case EQ:
        printf("EQ ");
        break;
    case NEQ:
        printf("NEQ ");
        break;
    case LT:
        printf("LT ");
        break;
    case GT:
        printf("GT ");
        break;
    case AND:
        printf("AND ");
        break;
    case OR:
        printf("OR ");
        break;
    case XOR:
        printf("XOR ");
        break;
    case SHR:
        printf("SHR ");
        break;
    case SHL:
        printf("SHL ");
        break;
    case ADD:
        printf("ADD ");
        break;
    case SUB:
        printf("SUB ");
        break;
    case MUL:
        printf("MUL ");
        break;
    case DIV:
        printf("DIV ");
        break;
    case MOD:
        printf("MOD ");
        break;
    case LDC:
        printf("LDC ");
        break;
    case STRC:
        printf("STRC ");
        break;
    case LDB:
        printf("LDB ");
        break;
    case STRB:
        printf("STRB ");
        break;
    case LDS:
        printf("LDS ");
        break;
    case STRS:
        printf("STRS ");
        break;
    case LDP:
        printf("LDP ");
        break;
    case STRP:
        printf("STRP ");
        break;
    case LDR:
        printf("LDR ");
        break;
    case STRR:
        printf("STRR ");
        break;
    case LDI:
        printf("LDI ");
        break;
    case STRI:
        printf("STRI ");
        break;
    case LDH:
        printf("LDH ");
        break;
    case STRH:
        printf("STRH ");
        break;
    case LDL:
        printf("LDL ");
        break;
    case STRL:
        printf("STRL ");
        break;
    case CSZ:
        printf("CSZ ");
        break;
    case CFUN:
        printf("CFUN ");
        break;
    case KEY:
        printf("KEY ");
        break;
    case EMIT:
        printf("EMIT ");
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

        if(*((cell *) &vm->mem[addr]) == LIT) {
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



