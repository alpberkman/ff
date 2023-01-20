
: APP LDH STRB LDH 1 ADD STRH ;


: LIT
	LIT LDH STRB
	LDH 1 ADD STRH
	LDH STRC
	LDH CSZ ADD STRH
;

: NOP NOP APP ;


: JMP
	LDH 1 ADD
	666 LIT
	JMP APP
;

: JZ
	LDH 1 ADD
	666 LIT
	JZ APP
;


: CALL CALL APP ;


: RET RET APP ;


: LEQ 1 PICKP 1 PICKP EQ PUSH LT POP OR ;
: GEQ 1 PICKP 1 PICKP EQ PUSH GT POP OR ;
: BET PUSH 1 PICKP LEQ SWAP POP LEQ AND ;


: IMMEDIATE LDL CSZ ADD DUP LDB 1 6 SHL OR SWAP STRB ;
: VISIBLE LDL CSZ ADD DUP LDB 1 7 SHL OR SWAP STRB ; IMMEDIATE
: SELF
	LDL CSZ ADD LDB 31 AND 
	LDL CSZ ADD 1 ADD ADD
	JMP STRC
; IMMEDIATE


: IF JZ ; IMMEDIATE
: THEN LDH SWAP STRC ; IMMEDIATE
: ELSE JMP SWAP LDH SWAP STRC ; IMMEDIATE


: ( KEY 41 NEQ IF SELF THEN ; IMMEDIATE
: \ KEY 10 NEQ IF SELF THEN ; IMMEDIATE
: WARN KEY
	DUP 92 EQ IF
		KEY DUP 10 EQ IF
			32 EMIT DROP DROP
		ELSE
			SWAP EMIT EMIT
		THEN
		SELF
	THEN
	DUP EMIT 10 NEQ IF SELF THEN
; IMMEDIATE

: EXIT RET ; IMMEDIATE
: BYE HALT ;


: ALLOT LDH ADD STRH ;


: BEGIN LDH ; IMMEDIATE
: AGAIN JMP STRC ; IMMEDIATE
: UNTIL JZ STRC ; IMMEDIATE
: WHILE JZ ; IMMEDIATE
: REPEAT SWAP JMP STRC LDH SWAP STRC ; IMMEDIATE


: LAST LDL ;
: NEXT LDC ;
: N-NEXT 0 LAST PUSH
	BEGIN 1 PICKP 1 PICKP GT WHILE
		POP NEXT PUSH 1 ADD
	REPEAT DROP DROP POP
;
: FORGET LDL LDC STRL ;


: DUMP DUP
	CSZ ADD DUP LDB 31 AND SWAP 1 ADD
	SWAP 1 PICKP ADD SWAP
	BEGIN 1 PICKP 1 PICKP GT WHILE DUP LDB EMIT 1 ADD REPEAT
	32 EMIT
	DROP DROP
	DUP 0 NEQ IF LDC SELF ELSE DROP THEN
;
: DUMP LDL DUMP 10 EMIT ;
FORGET FORGET


: CR ( --) 13 EMIT 10 EMIT ;


: . 48 EMIT 120 EMIT PUSH CSZ 8 MUL 0
	BEGIN 1 PICKP 1 PICKP GT WHILE
		POP DUP 4 SHL PUSH 
		CSZ 8 MUL 4 SUB SHR 15 AND 
		DUP 0 9 BET IF 48 ELSE 55 THEN 
		ADD EMIT
		4 ADD
	REPEAT POP DROP DROP DROP
;

: N-INVIS N-NEXT CSZ ADD DUP LDB 1 7 SHL XOR SWAP STRB ;


: NOT -1 XOR ;






