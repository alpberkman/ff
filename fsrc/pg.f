
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



: [CHAR] KEY LIT ; IMMEDIATE
: SPACE [CHAR]   EMIT ;
: ISDIGIT [CHAR] 0 [CHAR] 9 bet ;








































