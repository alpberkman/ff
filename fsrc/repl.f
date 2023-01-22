

: ILIT LIT ; IMMEDIATE


LDH 80 ALLOT
: TIB ILIT ;

LDH CSZ ALLOT
: #TIB ILIT ;

LDH CSZ ALLOT
: SPAN ILIT ;

: ISSPACE ( n -- flag )
    0
    1 PICKP 32 EQ OR
    1 PICKP  9 EQ OR
    1 PICKP 10 EQ OR
    1 PICKP 11 EQ OR
    1 PICKP 12 EQ OR
    1 PICKP 13 EQ OR
    SWAP DROP
;

: EXPECT ( addr +n -- )
    0 SPAN STRC
    DUP 0 EQ IF DROP DROP EXIT THEN
    BEGIN DUP SPAN LDC GT WHILE
        KEY DUP ISSPACE IF DROP DROP DROP EXIT THEN
        2 PICKP SPAN LDC ADD STRB
        SPAN LDC 1 ADD SPAN STRC
    REPEAT DROP DROP
;

: TYPE ( addr +n -- )
    DUP 0 EQ IF DROP DROP EXIT THEN
    1 PICKP ADD SWAP
    BEGIN 1 PICKP 1 PICKP GT WHILE
        DUP LDB EMIT 1 ADD
    REPEAT DROP DROP
;

: TOUPPER DUP 97 122 bet IF 32 SUB THEN ;
: TOLOWER DUP 65 90 bet IF 32 ADD THEN ;
(
LDH 80 ALLOT
DUP 10 expect 1234567 deb
SPAN LDC TYPE CR
)

: MEMCMP ( addr1 addr2 +n -- flag )
	0
	BEGIN 1 PICKP 1 PICKP GT WHILE
	3 PICKP 1 PICKP ADD LDB
	3 PICKP 2 PICKP ADD LDB 
	NEQ IF DROP DROP DROP DROP FALSE EXIT THEN
	1 ADD
	REPEAT DROP DROP DROP DROP TRUE
;

: ISDIGIT 48 57 bet ;
: ISNUM ( addr +n -- flag )
	DUP 0 EQ IF DROP DROP FALSE EXIT THEN
	1 PICKP LDB 45 EQ
	2 PICKP LDB 43 EQ OR
	IF 1 ELSE 0 THEN
	BEGIN 1 PICKP 1 PICKP GT WHILE
		DUP 3 PICKP ADD LDB ISDIGIT -1 XOR IF
			DROP DROP DROP FALSE EXIT
		THEN 1 ADD
	REPEAT DROP DROP DROP TRUE
;
: TONUM ( addr +n -- w )
	1 PICKP LDB 45 EQ
	2 PICKP LDB 43 EQ 
	OR IF 1 ELSE 0 THEN 0
	BEGIN 2 PICKP 2 PICKP GT WHILE
		10 MUL 3 PICKP 2 PICKP ADD LDB 48 SUB ADD
		PUSH 1 ADD POP 
	REPEAT
	3 PICKP LDB 45 EQ IF -1 MUL THEN
	PUSH DROP DROP DROP POP
;

: NUM ( addr +n -- w? flag )
	1 PICKP 1 PICKP ISNUM DUP PUSH
	IF TONUM ELSE DROP DROP THEN POP
;

\ clear
LDH 40
DUP ALLOT
1 PICKP 1 PICKP EXPECT -12112
1 PICKP SPAN LDC TYPE CR
1 PICKP SPAN LDC num

deb




