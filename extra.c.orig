/*
    COL, SEMI, INTERP, DEB,
*/

/*
void _col(VM *vm) {

    char buf[32];
    int c;
    while(isspace(c = getchar()));

    byte len = 0;
    do {
        if(c == EOF)
            return;
        buf[len++] = c - (c >= 'a' && c <= 'z' ? 'a' - 'A' : 0);
    } while(len < 31 && !isspace(c = getchar()));
    buf[len] = '\0';

    vm->s = COMPILE;

    *((cell *) &(vm->mem[vm->hp])) = *((cell *) &(vm->lp));
    vm->lp = vm->hp;
    vm->hp += CELL_SIZE;

    byte i;
    for(i = 0; buf[i] != '\0'; ++i)
        vm->mem[vm->hp + 1 + i] = buf[i];

    vm->mem[vm->hp] = i;
    vm->hp += 1 + i;
}

void _semi(VM *vm) {
    vm->mem[vm->hp++] = RET;
    vm->mem[vm->lp + CELL_SIZE] |= MASK_VIS;
    vm->s = INTERPRET;
}

void _interp(VM *vm) {

    char buf[32];
    int c;
    while(isspace(c = getchar()));

    byte len = 0;
    do {
        if(c == EOF)
            return;
        buf[len++] = c - (c >= 'a' && c <= 'z' ? 'a' - 'A' : 0);
    } while(len < 31 && !isspace(c = getchar()));
    buf[len] = '\0';

    cell addr;
    byte flags;

    for(addr = vm->lp; addr != 0; addr = *((cell *) &(vm->mem[addr]))) {
        flags = vm->mem[addr + CELL_SIZE];
        if((flags & MASK_VIS) && len == (flags & WORD_LEN))
            if(strncmp(buf, (char *) &(vm->mem[addr + CELL_SIZE + 1]), len) == 0)
                break;
    }

    cell num = 0;
    byte nflag = 0;

    if(addr == 0) {
        nflag = strspn(buf, "1234567890-+") == len;
        if(nflag)
            num = atoi(buf);
    }

    if(vm->s == INTERPRET) {
        if(addr) {
            vm->rs[vm->rsp++] = vm->ip - 1;
            vm->ip = addr + CELL_SIZE + 1 + len;
        } else if(nflag) {
            vm->ps[vm->psp++] = num;
            vm->ip = vm->ip - 1;
        } else {
            vm->ip = vm->ip - 1;
            printf("Error in interpreter: word not found %s\n", buf);
        }
    } else if(vm->s == COMPILE) {
        if(addr) {
            if(flags & MASK_IMM) {
                vm->rs[vm->rsp++] = vm->ip - 1;
                vm->ip = addr + CELL_SIZE + 1 + len;
            } else {
                vm->mem[vm->hp++] = LIT;
                *((cell *) &(vm->mem[vm->hp])) = addr + CELL_SIZE + 1 + len;
                vm->hp += CELL_SIZE;
                vm->mem[vm->hp++] = CALL;
                vm->ip = vm->ip - 1;
            }
        } else if(nflag) {
            vm->mem[vm->hp++] = LIT;
            *((cell *) &(vm->mem[vm->hp])) = num;
            vm->hp += CELL_SIZE;
            vm->ip = vm->ip - 1;
        } else {
            vm->s = INTERPRET;
            vm->hp = vm->lp;
            vm->lp = *((cell *) &(vm->mem[vm->lp]));
            vm->ip = vm->ip - 1;
            vm->psp = 0;
            vm->rsp = 0;
            printf("Error in compiler: word not found %s\n", buf);
        }
    } else {
        vm->s = INTERPRET;
        vm->ip = vm->ip - 1;
        vm->psp = 0;
        vm->rsp = 0;
    }
}

void _deb(VM *vm) {
    printf("IP: %i  HP: %i  LP: %i\n", vm->ip, vm->hp, vm->lp);
    printf("PS: %6i\tRS: %6i\n", vm->psp, vm->rsp);
    for(int i = 0; i < (vm->psp > vm->rsp ? vm->psp : vm->rsp); ++i) {
        if(vm->psp > i)
            printf("%10i\t", vm->ps[i]);
        else
            printf("          \t");
        if(vm->rsp > i)
            printf("%10i\n", vm->rs[i]);
        else
            printf("          \n");
    }
    printf("\n\n");
}
*/

/*
		case COL: _col(vm); break;
		case SEMI: _semi(vm); break;
		case INTERP: _interp(vm); break;
		case DEB: _deb(vm); break;
*/

/*


void words(VM *vm) {

    char col_arr[] = {
        COL,
    };
    word(vm, ":", col_arr, sizeof(col_arr), MASK_VIS);

    char semi_arr[] = {
        SEMI,
    };
    word(vm, ";", semi_arr, sizeof(semi_arr), MASK_VIS | MASK_IMM);

    char app_arr[] = {
        LDH, STRB, LDH, LIT, 1, 0, ADD, STRH,
    };
    word(vm, "app", app_arr, sizeof(app_arr), MASK_VIS | MASK_IMM);

}
*/

#define streq(X, Y) (strcmp((X), (Y)) == 0)
void interp(VM *vm) {
	char buf[32];
    int c;
    
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
    
    puts(buf);
    
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
    		printf("Compile mode with %s\n", buf);
    	} else {
    		int	i;
    		*((cell *) &(vm->mem[vm->hp])) = *((cell *) &(vm->lp));
			vm->lp = vm->hp;
			vm->hp += CELL_SIZE;

			vm->mem[vm->hp++] = strlen(buf);

			for(i = 0; i < (short) strlen(buf); ++i)
				vm->mem[vm->hp + i] = buf[i];
			vm->hp += strlen(buf);
    	}
    	return;
    }
    
    if(streq(buf, ";")) {
    	if(vm->s == INTERPRET) {
    		vm->s = INTERPRET;
            vm->ip = 0;
            vm->psp = 0;
            vm->rsp = 0;
    		printf("Interpret mode with %s\n", buf);
    	} else {
		    *((cell *) &(vm->mem[vm->hp])) = RET;
		    vm->hp += CELL_SIZE;
			vm->mem[vm->lp + CELL_SIZE] |= MASK_VIS;
			vm->s = INTERPRET;
    	}
    	return;
    }
    
}
