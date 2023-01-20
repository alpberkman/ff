
: ILIT LIT ; IMMEDIATE

: DUMP DUP DUP CSZ ADD LDB 1 7 SHL AND IF
        CSZ ADD DUP LDB 31 AND SWAP 1 ADD \ Get the starting address and the length
        SWAP 1 PICKP ADD SWAP \ Calculate the end address
        BEGIN 1 PICKP 1 PICKP GT WHILE DUP LDB EMIT 1 ADD REPEAT \ Print the word
        32 EMIT \ Print space
        DROP DROP \ Remove the addresses 
	ELSE DROP THEN
	DUP 0 NEQ IF LDC SELF THEN DROP \ Call itself if it hasn' t reached the end of the list
;
: DUMP LDL DUMP 10 EMIT ;



: ideb deb ; immediate

clear
WARN Welcome to the Playground
dump cr



LDH 80 ALLOT
: TIB ILIT ;

LDH CSZ ALLOT
: #TIB ILIT ;

LDH CSZ ALLOT
: SPAN ILIT ;

: ISSPACE
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

LDH 80 ALLOT
DUP 10 expect 1234567 deb
SPAN LDC TYPE CR














