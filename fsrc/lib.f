
: ROUNDUP PUSH 1 SUB POP
	1 PICKP 1 PICKP MOD
	PUSH ADD POP SUB
;


: <+ DUP 0 GT IF PUSH ADD POP 1 SUB SELF THEN DROP ;
: <- DUP 0 GT IF PUSH ADD POP 1 SUB SELF THEN DROP SUB ;
: <* DUP 0 GT IF PUSH MUL POP 1 SUB SELF THEN DROP ;
: </ DUP 0 GT IF PUSH MUL POP 1 SUB SELF THEN DROP DIV ;

: <( LDP POP SWAP PUSH PUSH ;
: )> LDP POP POP SWAP PUSH SUB ;


: (( LDP POP SWAP PUSH PUSH ;
: )) LDP POP POP SWAP PUSH SUB ;

: +)) LDP POP POP SWAP PUSH SUB 1 SUB <+ ;
: -)) LDP POP POP SWAP PUSH SUB 2 SUB <- ;
: *)) LDP POP POP SWAP PUSH SUB 1 SUB <* ;
: /)) LDP POP POP SWAP PUSH SUB 2 SUB </ ;

\ (( 1 2 (( 12 3 2 /)) +))


: [CHAR]
	KEY DUP 92 EQ IF DROP KEY
		DUP 97 EQ IF DROP 7 THEN
		DUP 98 EQ IF DROP 8 THEN
		DUP 101 EQ IF DROP 27 THEN
		DUP 102 EQ IF DROP 12 THEN
		DUP 110 EQ IF DROP 10 THEN
		DUP 114 EQ IF DROP 13 THEN
		DUP 116 EQ IF DROP 9 THEN
		DUP 118 EQ IF DROP 11 THEN
		DUP 92 EQ IF DROP 92 THEN
		DUP 39 EQ IF DROP 39 THEN
		DUP 34 EQ IF DROP 34 THEN
		DUP 63 EQ IF DROP 63 THEN
		DUP 115 EQ IF DROP 32 THEN \ for space can use \s
	THEN LIT
; IMMEDIATE

: SPACE [CHAR]   EMIT ;
: ISDIGIT [CHAR] 0 [CHAR] 9 bet ;

