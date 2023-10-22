VOCABULARY SCM

: ENUM ( n -- n+1 ) DUP 1+ SWAP CONSTANT ;
: BYTE 1 ;
: BYTES BYTE * ;
: BYTE+ BYTE + ;
: ++ DUP @ 1+ SWAP ! ;
: -- DUP @ 1- SWAP ! ;
: ARRAY ( n -- )
  HERE SWAP ALLOT
  CONSTANT
;
: LIFO ( n -- )
  DUP , 0 , ARRAY
;
: LIFO-SIZE ( lifo -- size )
  2 CELLS -
;
: LIFO-CTR ( lifo -- ctr )
  1 CELLS -
;
: POP ( lifo -- n )
  DUP LIFO-CTR --
  DUP LIFO-CTR @ CELLS + @
;
: PUSH ( n lifo -- )
  DUP -ROT DUP LIFO-CTR @ + !
  LIFO-CTR ++
;
: LIFO-PRINT ( lifo -- )
  DUP LIFO-SIZE @ 0 DO
    DUP I CELLS + @ . CR
  LOOP DROP
;




\ Helpers and tags for lisp expressions
0 ENUM ATOM
  ENUM PRIM
  ENUM CONS
  ENUM CLOS
  ENUM NIL
  ENUM INT
DROP

: EXP ( -- )
  HERE 2 CELLS ALLOT
  CONSTANT
;
: VAL ( addr -- val ) ;
: TAG ( addr -- tag ) CELL+ ;


\ Memory for the interpreter
16384 BYTES CONSTANT MEM-SIZE
MEM-SIZE ARRAY MEM
VARIABLE HP
VARIABLE SP
EXP NIL
EXP TRU
EXP ERR
EXP ENV


: BOX ;
: BOX@ ( addr -- val tag )
  DUP @
  SWAP CELL+ @
;
: BOX! ( val tag addr -- )
  ROT OVER ! !
;

: EQU ( val1 tag1 val2 tag2 -- f )
  ROT = -ROT = AND
;

: NOT ( val tag -- f )
  NIL = NIP
;

: LISP-FIND ;
: ATOM ( addr n -- val tag)

;





























\ Initialize values
MEM MEM-SIZE 0 fill
0 HP !
MEM-SIZE SP !
