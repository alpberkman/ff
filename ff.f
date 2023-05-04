

: TRUE -1 ;
: FALSE 0 ;


: VIS 1 7 LSHIFT ;
: IMM 1 6 LSHIFT ;
: LEN 31 ;


: CELL+ CELL + ;
: CELL- CELL - ;
: CELLS CELL * ;


: IMMEDIATE LAST CELL+ DUP C@ IMM OR SWAP C! ;
: VISIBLE  LAST CELL+ DUP C@ VIS OR SWAP C! ; IMMEDIATE


: [ FALSE STATE! ; IMMEDIATE
: ] TRUE STATE! ;


: , HERE ! HERE CELL+ HERE! ;
: LITERAL POSTPONE LIT POSTPONE LIT , , ; IMMEDIATE


: [SELF] R> @ JMP ;
: SELF ['] [SELF] , LAST CELL+ DUP C@ LEN AND + 1 + , ; IMMEDIATE


: [IF] 5 CELLS IP + JZ R> CELL+ JMP R> @ JMP ;
: IF ['] [IF] , HERE HERE CELL+ HERE! ; IMMEDIATE


: [THEN] ;
: THEN ['] [THEN] , HERE SWAP ! ; IMMEDIATE


: [ELSE] R> @ JMP ;
: ELSE ['] [ELSE] , >R HERE HERE CELL+ HERE! R> HERE SWAP ! ; IMMEDIATE


: [BEGIN] ;
: BEGIN ['] [BEGIN] , HERE ; IMMEDIATE


: [AGAIN] R> @ JMP ;
: AGAIN ['] [AGAIN] , , ; IMMEDIATE


: [UNTIL] 5 CELLS IP + JZ R> CELL+ JMP R> @ JMP ;
: UNTIL ['] [UNTIL] , , ; IMMEDIATE


: [WHILE] 5 CELLS IP + JZ R> CELL+ JMP R> @ JMP ;
: WHILE ['] [WHILE] , HERE HERE CELL+ HERE! ; IMMEDIATE

: [REPEAT] R> @ JMP ;
: REPEAT ['] [REPEAT] , >R , R> HERE SWAP ! ; IMMEDIATE



: \ KEY 10 <> IF SELF THEN ; IMMEDIATE
: ( KEY 41 <> IF SELF THEN ; IMMEDIATE
: WARN KEY
	DUP 92 = IF
		KEY DUP 10 = IF
			32 EMIT DROP DROP
		ELSE
			SWAP EMIT EMIT
		THEN
		SELF
	THEN
	DUP EMIT 10 <> IF SELF THEN
; IMMEDIATE




: ISSPACE ( n -- flag )
    0
    1 PICK 32 = OR
    1 PICK  9 = OR
    1 PICK 10 = OR
    1 PICK 11 = OR
    1 PICK 12 = OR
    1 PICK 13 = OR
    SWAP DROP
;
: CHAR BEGIN KEY DUP ISSPACE WHILE DROP REPEAT ;
: [CHAR] CHAR POSTPONE LITERAL ; IMMEDIATE



: R@ R> DUP >R ;

\ THIS is a comment


: x if 2 then ;

WARN add postpone to the outer interpreter

