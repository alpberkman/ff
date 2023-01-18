
: ideb deb ; immediate


: VAR LDH CSZ LDH ADD STRH LDH SWAP : VISIBLE LIT RET STRL ;
WARN just compiling ret isnt\
enough i need to update\\ both lp and mark the word visible\
BROKEN DOENST WORK




: ." KEY DUP 34 NEQ IF APP SELF THEN DROP 0 APP ;
: ." JMP LDH ." SWAP LDH SWAP STRC LIT ; IMMEDIATE
: PRINT BEGIN DUP LDB 0 NEQ WHILE DUP LDB EMIT 1 ADD REPEAT DROP ;

\ void print(char *str) {
\ 	while(*c != 0) {
\ 		putchar(*c);
\ 		c += 1;
\ 	}
\ }

\ : print ( addr -- )
\ 	begin dup c@ 0 != while
\ 		dup c@ emit
\ 		1+
\ 	repeat
\ 	drop
\ ;



\ : PX LDS IF ." ELSE
\ : P DUP LDB DUP 0 EQ IF DROP DROP EXIT ELSE EMIT 1 ADD SELF THEN ;

( : ."
	." LIT
	LDH
	dup ldb dup 0 eq if drop drop exit then emit 1 add
	dup ldb emit 1 add
	JMP STRC
; IMMEDIATE

)

 : xxx ." asdfasdf" print ;
 deb xxx deb
( bye






)


















