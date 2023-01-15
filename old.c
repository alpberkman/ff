	// ' tick
	//
	
	
/*
	char immediate_arr[] = {
		LDL, LIT, CELL_SIZE, 0, ADD, DUP, LDB,
		LIT, (1<<6), 0, OR, SWAP, STRB
	};
	word(vm, "immediate", immediate_arr, sizeof(immediate_arr), MASK_VIS);
	
	
	char nop_arr[] = {
		LIT, NOP, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "nop", nop_arr, sizeof(nop_arr), MASK_VIS | MASK_IMM);
	
	char lit_arr[] = {
		LIT, LIT, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "lit", nop_arr, sizeof(lit_arr), MASK_VIS | MASK_IMM);
	
	char halt_arr[] = {
		LIT, HALT, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "halt", halt_arr, sizeof(halt_arr), MASK_VIS | MASK_IMM);
	
	
	char dup_arr[] = {
		DUP
	};
	word(vm, "dup", dup_arr, sizeof(dup_arr), MASK_VIS);
	
	char drop_arr[] = {
		DROP
	};
	word(vm, "drop", drop_arr, sizeof(drop_arr), MASK_VIS);
	
	char swap_arr[] = {
		SWAP
	};
	word(vm, "swap", swap_arr, sizeof(swap_arr), MASK_VIS);
	
	char push_arr[] = {
		PUSH
	};
	word(vm, "push", push_arr, sizeof(push_arr), MASK_VIS);
	
	char pop_arr[] = {
		POP
	};
	word(vm, "pop", pop_arr, sizeof(pop_arr), MASK_VIS);
	
	char pickp_arr[] = {
		PICKP
	};
	word(vm, "pickp", pickp_arr, sizeof(pickp_arr), MASK_VIS);
	
	char pickr_arr[] = {
		PICKR
	};
	word(vm, "pickr", pickr_arr, sizeof(pickr_arr), MASK_VIS);
	
	
	char jmp_arr[] = {
		LIT, JMP, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "jmp", jmp_arr, sizeof(jmp_arr), MASK_VIS | MASK_IMM);
	
	char jz_arr[] = {
		LIT, JZ, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "jz", jz_arr, sizeof(jz_arr), MASK_VIS | MASK_IMM);
	
	
	char call_arr[] = {
		LIT, CALL, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "call", call_arr, sizeof(call_arr), MASK_VIS | MASK_IMM);
	
	char ret_arr[] = {
		LIT, RET, 0, LDH, STRB,
		LIT, 1, 0, LDH, STRH
	};
	word(vm, "ret", ret_arr, sizeof(ret_arr), MASK_VIS | MASK_IMM);
	
	
	char eq_arr[] = {
		EQ
	};
	word(vm, "eq", eq_arr, sizeof(eq_arr), MASK_VIS);
	
	char neq_arr[] = {
		NEQ
	};
	word(vm, "neq", neq_arr, sizeof(neq_arr), MASK_VIS);
	
	char lt_arr[] = {
		LT
	};
	word(vm, "lt", lt_arr, sizeof(lt_arr), MASK_VIS);
	
	char gt_arr[] = {
		GT
	};
	word(vm, "gt", gt_arr, sizeof(gt_arr), MASK_VIS);
	
	
	
	char _arr[] = {
		
	};
	word(vm, "", _arr, sizeof(_arr), MASK_VIS);


    AND, OR, XOR,
    SHR, SHL,

    ADD, SUB, MUL,
    DIV, MOD,

    LDC, STRC,
    LDB, STRB,

    LDS, STRS,

    LDP, STRP,
    LDR, STRR,

    LDI, STRI,
    LDH, STRH,
    LDL, STRL,

    KEY, EMIT,
	*/