
#define streq(X, Y) (strcmp((X), (Y)) == 0)
#define err(X, Y) fprintf(stderr, "ERROR: %s %s\n", (X), (Y))


void scanword(char *buf, byte *len) {
    int c;

    /*Remove white space*/
    while(isspace(c = getchar()));

    /*Get the string*/
    *len = 0;
    do {
        if(c == EOF)
            return;
        buf[(*len)++] = toupper(c);
    } while(*len < 31 && !isspace(c = getchar()));
    buf[*len] = '\0';
}


void interp(VM *vm) {
    char buf[32];
    cell addr = 0;
    cell num = 0;
    byte flags = 0;
    byte nflag = 0;
    byte len;

    scanword(buf, &len);

    if(streq(buf, "DEBUG")) {
        debug(vm);
        return;
    }

    if(streq(buf, ":")) {
        if(vm->s == COMPILE) {
            vm->s = INTERPRET;
            vm->hp = vm->lp;
            vm->lp = *((cell *) &(vm->mem[vm->lp]));
            vm->ip = 0;
            vm->psp = 0;
            vm->rsp = 0;
            err(buf, "in COMPILE mode");
        } else {
            int	i;

            scanword(buf, &len);

            if(streq(buf, ":")||streq(buf, ";")||streq(buf, "DEBUG")||streq(buf, "\'")||streq(buf, "POSTPONE")) {
                vm->s = INTERPRET;
                vm->ip = 0;
                vm->psp = 0;
                vm->rsp = 0;
                err(buf, "in COMPILE mode");
                return;
            }

            *((cell *) &(vm->mem[vm->hp])) = *((cell *) &(vm->lp));
            vm->lp = vm->hp;
            vm->hp += CELL_SIZE;

            vm->mem[vm->hp++] = strlen(buf);

            for(i = 0; i < (int) strlen(buf); ++i)
                vm->mem[vm->hp + i] = buf[i];
            vm->hp += strlen(buf);

            vm->s = COMPILE;
        }
        return;
    }

    if(streq(buf, ";")) {
        if(vm->s == INTERPRET) {
            vm->s = INTERPRET;
            vm->ip = 0;
            vm->psp = 0;
            vm->rsp = 0;
            err(buf, "in INTERPRET mode");
        } else {
            vm->s = INTERPRET;
            *((cell *) &(vm->mem[vm->hp])) = 0xb3 + 7;/*RET; WONT WORK FOR 32 bit*/
            vm->hp += CELL_SIZE;
            vm->mem[vm->lp + CELL_SIZE] |= MASK_VIS;
            vm->ip = 0; /*WARN*/
        }
        return;
    }

    if(streq(buf, "\'")) {
        if(vm->s == (cell)COMPILE) {
            vm->s = INTERPRET;
            vm->ip = 0;
            vm->psp = 0;
            vm->rsp = 0;
            err(buf, "in COMPILE mode");
        } else {
            cell addr;

            scanword(buf, &len);


            if(streq(buf, ":")||streq(buf, ";")||streq(buf, "DEBUG")||streq(buf, "\'")||streq(buf, "POSTPONE")) {
                vm->s = INTERPRET;
                vm->ip = 0;
                vm->psp = 0;
                vm->rsp = 0;
                err(buf, "in COMPILE mode");
                return;
            }

            for(addr = vm->lp; addr != 0; addr = *((cell *) &(vm->mem[addr]))) {
                flags = vm->mem[addr + CELL_SIZE];
                if((flags & MASK_VIS) && len == (flags & WORD_LEN))
                    if(strncmp(buf, (char *) &(vm->mem[addr + CELL_SIZE + 1]), len) == 0)
                        break;
            }

            if(addr == 0) {
                err(buf, "not found in \'");
                return;
            }

            vm->ps[vm->psp++] = addr + CELL_SIZE + 1 + len;

        }
        return;
    }

    if(streq(buf, "[\']")) {
        if(vm->s == (cell)INTERPRET) {
            vm->s = INTERPRET;
            vm->ip = 0;
            vm->psp = 0;
            vm->rsp = 0;
            err(buf, "in INTERPRET mode");
        } else {
            cell addr;

            scanword(buf, &len);


            if(streq(buf, ":")||streq(buf, ";")||streq(buf, "DEBUG")||streq(buf, "\'")||streq(buf, "POSTPONE")) {
                vm->s = INTERPRET;
                vm->ip = 0;
                vm->psp = 0;
                vm->rsp = 0;
                err(buf, "in COMPILE mode");
                return;
            }

            for(addr = vm->lp; addr != 0; addr = *((cell *) &(vm->mem[addr]))) {
                flags = vm->mem[addr + CELL_SIZE];
                if((flags & MASK_VIS) && len == (flags & WORD_LEN))
                    if(strncmp(buf, (char *) &(vm->mem[addr + CELL_SIZE + 1]), len) == 0)
                        break;
            }

            if(addr == 0) {
                err(buf, "not found in \'");
                return;
            }

            *((cell *) &(vm->mem[vm->hp])) = 0x0c + 6;
            vm->hp += CELL_SIZE;
            *((cell *) &(vm->mem[vm->hp])) = addr + CELL_SIZE + 1 + len;
            vm->hp += CELL_SIZE;


        }
        return;
    }

    if(streq(buf, "POSTPONE")) {
        if(vm->s == (cell)INTERPRET) {
            vm->s = INTERPRET;
            vm->ip = 0;
            printf("ERROR: INTERPRET mode with %s\n", buf);
        } else {
            cell addr;

            scanword(buf, &len);


            if(streq(buf, ":")||streq(buf, ";")||streq(buf, "DEBUG")||streq(buf, "\'")||streq(buf, "POSTPONE")) {
                vm->s = INTERPRET;
                vm->ip = 0;
                vm->psp = 0;
                vm->rsp = 0;
                printf("ERROR: Compile mode name %s\n", buf);
                return;
            }

            for(addr = vm->lp; addr != 0; addr = *((cell *) &(vm->mem[addr]))) {
                flags = vm->mem[addr + CELL_SIZE];
                if((flags & MASK_VIS) && len == (flags & WORD_LEN))
                    if(strncmp(buf, (char *) &(vm->mem[addr + CELL_SIZE + 1]), len) == 0)
                        break;
            }

            if(addr == 0) {
                printf("ERROR: COMPILE mode ' name not found %s\n", buf);
                return;
            }

            *((cell *) &(vm->mem[vm->hp])) = addr + CELL_SIZE + 1 + len;
            vm->hp += CELL_SIZE;

        }
        return;
    }

    for(addr = vm->lp; addr != 0; addr = *((cell *) &(vm->mem[addr]))) {
        flags = vm->mem[addr + CELL_SIZE];
        if((flags & MASK_VIS) && len == (flags & WORD_LEN))
            if(strncmp(buf, (char *) &(vm->mem[addr + CELL_SIZE + 1]), len) == 0)
                break;
    }

    if(addr == 0) {
        nflag = strspn(buf, "1234567890-+") == len;
        if(nflag)
            num = atoi(buf);
    }

    /*
    printf("%s : addr: %i num: %i flags: %i nflag: %i\n", buf, addr, num, flags, nflag);
    */
    if(vm->s == INTERPRET) {
        if(addr) {
            *((cell *) &(vm->mem[0])) = addr + CELL_SIZE + 1 + len;
            vm->ip = 0;
            vm->p = ON;
        } else if(nflag) {
            vm->ps[vm->psp++] = num;
        } else {
            printf("Error in interpreter: word not found %s\n", buf);
        }
    } else if(vm->s == COMPILE) {
        if(addr) {
            if(flags & MASK_IMM) {
                *((cell *) &(vm->mem[0])) = addr + CELL_SIZE + 1 + len;
                vm->ip = 0;
                vm->p = ON;
            } else {
                *((cell *) &(vm->mem[vm->hp])) = addr + CELL_SIZE + 1 + len;
                vm->hp += CELL_SIZE;
            }
        } else if(nflag) {
            *((cell *) &(vm->mem[vm->hp])) = 0x0c + 6; /*LIT;*/
            vm->hp += CELL_SIZE;
            *((cell *) &(vm->mem[vm->hp])) = num;
            vm->hp += CELL_SIZE;
        } else {
            vm->s = INTERPRET;
            vm->hp = vm->lp;
            vm->lp = *((cell *) &(vm->mem[vm->lp]));
            printf("Error in compiler: word not found %s\n", buf);
        }
    } else {
        vm->s = INTERPRET;
        vm->ip = 0;
        vm->p = OFF;
        vm->psp = 0;
        vm->rsp = 0;
    }
}
