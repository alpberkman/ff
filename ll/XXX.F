VOCABULARY SCM

: ENUM ( n -- n+1 ) DUP 1+ SWAP CONSTANT ;
: BYTE 1 ;
: BYTES BYTE * ;
: BYTE+ BYTE + ;
: ARRAY ( n -- )
  HERE SWAP ALLOT
  CONSTANT
;


\ Helpers and tags for lisp expressions
0 ENUM ATOM
  ENUM PRIM
  ENUM CONS
  ENUM CLOS
  ENUM NIL
  ENUM INT
DROP

\ : EXP ( -- ) 2 CELLS ALLOT ;
: VAL ( addr -- val ) @ ;
: TAG ( addr -- tag ) CELL+ @ ;


\ Memory for the interpreter
16384 BYTES CONSTANT MEM-SIZE
MEM-SIZE ARRAY MEM
VARIABLE HP
VARIABLE SP
VARIABLE NIL
VARIABLE TRU
VARIABLE ERR
VARIABLE ENV


: EQU ( addr1 addr2 -- f )
  OVER VAL OVER VAL = -ROT
  ROT SWAP ROT AND
;

: NOT ( )


: STRLEN ( addr -- n )
  0 BEGIN OVER OVER + @ 0 <> WHILE
    1+
  REPEAT NIP
;
: STRCMP ( addr1 addr2 -- n )
  0 BEGIN
    2 PICK
  WHILE
    DROP 1+
  AGAIN
  NIP NIP NIP
;
