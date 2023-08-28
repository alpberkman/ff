

: ISUPPER ( n -- flag ) [CHAR] A [CHAR] Z <=<= ;
: ISLOWER ( n -- flag ) [CHAR] a [CHAR] z <=<= ;



: TOLOWER ( n -- n )
        DUP ISUPPER IF [ CHAR a CHAR A - ] LITERAL + THEN
;
: TOUPPER ( n -- n )
        DUP ISLOWER IF [ CHAR a CHAR A - ] LITERAL - THEN
;




: FMT KEY TOUPPER DUP EMIT
    [CHAR] ; = IF
        KEY TOUPPER DUP EMIT
        LF = IF
            EXIT
        THEN
    THEN
    SELF
;


