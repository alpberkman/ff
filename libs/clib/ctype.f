


: ISUPPER ( n -- flag ) [CHAR] A [CHAR] Z <=<= ;
: ISLOWER ( n -- flag ) [CHAR] a [CHAR] z <=<= ;
: ISALPHA ( n -- flag ) DUP ISUPPER SWAP ISLOWER OR ;
: ISDIGIT ( n -- flag ) [CHAR] 0 [CHAR] 9 <=<= ;
: ISALNUM ( n -- flag ) DUP ISALPHA SWAP ISDIGIT OR ;
: ISXDIGIT ( n -- flag )
	0
	OVER [CHAR] a [CHAR] f <=<= OR
	OVER [CHAR] A [CHAR] F <=<= OR
	OVER ISDIGIT OR
	SWAP DROP
;
: ISSPACE ( n -- flag )
    0
    OVER 9 13 <=<= OR
    OVER BL = OR
    SWAP DROP
;
: ISCNTRL ( n -- flag )
	0
	OVER 0 31 <=<= OR
	OVER 127 = OR
	SWAP DROP
;



: TOLOWER ( n -- n )
	DUP ISUPPER IF [ CHAR a CHAR A - ] LITERAL + THEN
;
: TOUPPER ( n -- n )
	DUP ISLOWER IF [ CHAR a CHAR A - ] LITERAL - THEN
;




5 	int isgraph(int c)

This function checks whether the passed character has graphical representation using locale.

7 	int isprint(int c)

This function checks whether the passed character is printable.
8 	int ispunct(int c)

This function checks whether the passed character is a punctuation character.




