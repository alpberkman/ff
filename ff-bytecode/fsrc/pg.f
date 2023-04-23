
: ILIT LIT ; IMMEDIATE
: TOLOWER DUP 65 90 bet IF 32 ADD THEN ;
: DUMP DUP DUP CSZ ADD LDB 1 7 SHL AND IF
        CSZ ADD DUP LDB 31 AND SWAP 1 ADD \ Get the starting address and the length
        SWAP 1 PICKP ADD SWAP \ Calculate the end address
        BEGIN 1 PICKP 1 PICKP GT WHILE DUP LDB tolower EMIT 1 ADD REPEAT \ Print the word
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


: [ 0 STRS ; IMMEDIATE
: ] 1 STRS ;

LDH 80 ALLOT
: TIB ( -- addr ) [ LIT ] ;

LDH CSZ ALLOT
: #TIB ( -- addr ) [ LIT ] ;

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

: READ ( -- )
    0 #TIB STRC
    BEGIN KEY DUP ISSPACE WHILE DROP REPEAT
    BEGIN
        #TIB LDC TIB ADD STRB
        #TIB LDC 1 ADD #TIB STRC
        #TIB LDC 31 EQ IF EXIT THEN
        KEY DUP ISSPACE IF DROP EXIT THEN
    AGAIN
;

: TYPE ( addr +n -- )
    DUP 0 EQ IF DROP DROP EXIT THEN
    1 PICKP ADD SWAP
    BEGIN 1 PICKP 1 PICKP GT WHILE
        DUP LDB EMIT 1 ADD
    REPEAT DROP DROP
;

read    asdfasdf
deb
TIB #TIB LDC TYPE cr


: FIND ( addr +n -- addr? flag )
    BEGIN DUP 0 NEQ WHILE
        DUP CSZ ADD LDB 31 AND PUSH
        DUP CSZ ADD 1 ADD POP
        1 PICKP 1 PICKP ADD . 58 emit 32 emit TYPE cr
    LDC REPEAT DROP
;

cr cr cr
ldl find cr























