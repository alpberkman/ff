// How about pushm memory push
#define BAPP LDH, STRB, LDH, LIT, 1, 0, ADD, STRH
#define CAPP LDH, STRC, LDH, LIT, CELL_SIZE, 0, ADD, STRH
#define BEQ(X) LIT, X, 0, EQ
#define ISSPACE LIT, 0, 0, \
				OVER, BEQ(' '), OR, \
				OVER, BEQ('\t'), OR, \
				OVER, BEQ('\n'), OR, \
				OVER, BEQ('\v'), OR, \
				OVER, BEQ('\f'), OR, \
				OVER, BEQ('\r'), OR
#define NOT LIT, 0, 0, NEQ
					
void words(VM *vm) {

	char app_arr[] = {
		BAPP
	};
	word(vm, "app", app_arr, sizeof(app_arr), MASK_VIS | MASK_IMM);

	char semi_colon_arr[] = {
		LIT, RET, 0, BAPP,
		LDL, LIT, CELL_SIZE, 0, ADD, DUP, LDB,
		LIT, MASK_VIS, 0, OR, SWAP, STRB
	};
	word(vm, "semi_colon", semi_colon_arr, sizeof(semi_colon_arr), MASK_VIS | MASK_IMM);
	
	char colon_arr[] = {
		LIT, COMPILE, 0, STRS,	// Set the VM to compilation mode
		LDL, CAPP, LIT, 0, 0, BAPP,	// Add the last address to the new word, also init the header with 0
		DUP,
		DROP, KEY, ISSPACE,
		NOT, LIT, /*addr drop before key*/,JZ,	// loop till we get an actual letter
		BAPP, LIT, 1, 0,
		DUP, LIT, 31, 0, EQ, NOT, LIT, /*addr after2*/, JZ,
		KEY, ISSPACE, NOT, LIT, /*addr after1*/, JZ,
		BAPP,
		DROP
		
		
	};
	word(vm, "colon", colon_arr, sizeof(colon_arr), MASK_VIS);
	
	char repl_arr[] = {
		
	};
	word(vm, "repl", repl_arr, sizeof(repl_arr), MASK_VIS);
	

}

void _con(VM *vm) {
	_col(vm);
	vm->mem[vm->hp++] = LIT;
	*((cell *) &(vm->mem[vm->hp])) = vm->ps[--vm->psp];
	vm->hp += sizeof(cell);
	_semi(vm);
}

void _var(VM *vm) {
	_col(vm);
	vm->mem[vm->hp++] = LIT;
	*((cell *) &(vm->mem[vm->hp])) = vm->hp + sizeof(cell) + 1;
	vm->hp += sizeof(cell);
	vm->mem[vm->hp++] = RET;
	vm->hp += sizeof(cell);
	vm->mem[vm->lp + sizeof(cell)] |= MASK_VIS;
	vm->s = INTERPRET;
}


void test(unsigned char *arr, int size) {
    VM vm;
    init(&vm);
    memcpy(vm.mem, arr, size);
    vm.hp = size;
    run(&vm);
    debug(&vm);
}

void testall(void) {

    unsigned char arr1[] = {
        LIT,1,0,LIT,5,0,
        DUP,MUL,ADD,
        DUP,NOP,NOP,NOP,
        LIT,5,0,ADD,SWAP,DROP,
        DUP,PUSH,
        LIT,-1,-1,MUL,
        POP,
        PUSH,
        LIT,0,0,PICKP,
        LIT,0,0,PICKR,
        LIT,1,0,PICKP,
        HALT
    };
    test(arr1, sizeof(arr1));

    unsigned char arr2[] = {
        LIT,0,0,LIT,-1,-1,XOR,
        LIT,1,0,AND,DUP,
        LIT,2,0,OR,
        LIT,1,0,SHR,
        LIT,3,0,SHL,
        ADD,
        LIT,5,0,SUB,
        LIT,3,0,MUL,
        DUP,
        LIT,7,0,DIV,
        SWAP,
        LIT,7,0,MOD,
        LIT,0,0,LIT,1,0,EQ,
        LIT,0,0,LIT,0,0,EQ,
        LIT,0,0,LIT,1,0,NEQ,
        LIT,0,0,LIT,0,0,NEQ,
        LIT,0,0,LIT,1,0,LT,
        LIT,0,0,LIT,1,0,SWAP,LT,
        LIT,0,0,LIT,1,0,GT,
        LIT,0,0,LIT,1,0,SWAP,GT,
        HALT
    };
    test(arr2, sizeof(arr2));

    unsigned char arr3[] = {
        LIT,1,0,
        LIT,10,0,JZ,
        LIT,3,3,
        LIT,3,3,
        LIT,3,3,
        HALT
    };
    test(arr3, sizeof(arr3));

    unsigned char arr4[] = {
        LIT,7,0,CALL,
        LIT,3,3,
        LIT,3,3,
        LIT,3,3,
        
        LIT,21,0,PUSH,RET,
        LIT,3,3,
        LIT,3,3,
        LIT,3,3,
        
        HALT
    };
    test(arr4, sizeof(arr4));

    unsigned char arr5[] = {
        NOP,NOP,NOP,NOP,
        LDI,DUP,DUP,
        LIT,0,0,STRB,
        LIT,1,0,STRB,
        LIT,2,0,STRC,
        HALT
    };
    test(arr5, sizeof(arr5));
}

/*
// How about pushm memory push
#define BAPP LDH, STRB, LDH, LIT, 1, 0, ADD, STRH
#define CAPP LDH, STRC, LDH, LIT, sizeof(cell), 0, ADD, STRH
#define BEQ(X) LIT, X, 0, EQ
#define ISSPACE LIT, 0, 0, \
				OVER, BEQ(' '), OR, \
				OVER, BEQ('\t'), OR, \
				OVER, BEQ('\n'), OR, \
				OVER, BEQ('\v'), OR, \
				OVER, BEQ('\f'), OR, \
				OVER, BEQ('\r'), OR
#define NOT LIT, 0, 0, NEQ
					
void words(VM *vm) {

	char app_arr[] = {
		BAPP
	};
	word(vm, "app", app_arr, sizeof(app_arr), MASK_VIS | MASK_IMM);
}

void repl(VM *vm) {
	char buf[MAX_LEN + 1];
	while(scanf("%31s", buf) != EOF) {
		if(vm->s == INTERPRET)
			interp(vm, buf);
		else
			comp(vm, buf);
		run(vm);
		vm->p = ON;
	}
}

void interp(VM *vm, char *word) {}
void comp(VM *vm, char *word) {}
*/


