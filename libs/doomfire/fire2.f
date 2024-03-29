: ARRAY ( n -- )
  CREATE CELLS HERE SWAP DUP ALLOT 0 FILL
  DOES> SWAP CELLS +
;
2080 CONSTANT BUF-SIZE
BUF-SIZE ARRAY BUF

: ESC 27 EMIT ;
: CLEAR ESC 99 EMIT ;

VARIABLE RSEED
: SRAND ( n -- )
  RSEED !
;
: RAND ( -- n )
  RSEED @
  13 + 123 * 157 -
  DUP RSEED !
;

: TICK 
  BUF-SIZE 1 DO

  LOOP
;

: PRINT
  CLEAR
  BUF-SIZE 0 DO
    I 80 MOD 79 = IF
      CR
    ELSE
      S"  .,-~:;=!*#$@" DROP
      I BUF @ + C@ EMIT
    THEN
  LOOP
;

: MAIN BEGIN TICK PRINT AGAIN ;
