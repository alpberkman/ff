
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
	THEN LIT
; IMMEDIATE

: SPACE [CHAR]   EMIT ;
: ISDIGIT [CHAR] 0 [CHAR] 9 bet ;








































