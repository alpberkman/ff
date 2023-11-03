VARIABLE READ-CTR

: APPEND ( char addr -- )
  READ-CTR @ + C!
  READ-CTR ++
;

: READ ( addr n -- )
  0 READ-CTR !
  BEGIN KEY? 0= UNTIL
  2 PICK APPEND
  1 DO
    KEY? ?LEAVE
    OVER APPEND
  LOOP DROP
;

: FIND ( addr -- addr 0 | xt 1 | xt -1 )
  LAST BEGIN ?DUP WHILE
    OVER OVER DUP NAME SWAP FLAGS LEN? STREQ IF
    NIP DUP CODE SWAP FLAGS IMM? IF 1 ELSE -1 THEN EXIT THEN
  PREV REPEAT
;

: VOCAB
  LAST BEGIN ?DUP WHILE
    DUP NAME OVER FLAGS LEN? TYPE
    BL EMIT
  PREV REPEAT CR
;

(

: ?? POSTPONE DUP
     POSTPONE IF
     POSTPONE ['] POSTPONE EXECUTE
     POSTPONE THEN
; immediate
)

\ : a ?? dup ;

: in tib 31 read ;
: out tib read-ctr @ type cr ;




: DIVISIBLE ( u1 u2 -- u1 u2 f ) OVER OVER % 0 = ;


: B2C DUP BYTE * CELL / SWAP CELL % + ;
: C2B CELL * BYTE / ;

: R>> R> R> ;
: R<< R< R< ;


: [FOR] BURY 0 BURY ;
: FOR
  ['] [FOR] ,
  HERE
; IMMEDIATE

: [NEXT]
  1 I+
  1 RICK 2 RICK = BRANCH0-UNLOOP
;
: NEXT
  ['] [NEXT] ,
  ,
; IMMEDIATE

: [(] BRANCH ;
: (
  ['] [(] ,
  [CHAR] ( C,
  HERE CELL ALLOT
  BEGIN
  KEY DUP C,
  [CHAR] ) = UNTIL
  HERE !
; IMMEDIATE


: OFF FALSE SWAP ! ;
: ON TRUE SWAP ! ;


: BETWEEN  ( n min max -- flag ) 1+ WITHIN ;


/ unneeded
: STRLEN ( addr -- n )
  0 BEGIN OVER OVER + @ 0 <> WHILE
    1+
  REPEAT NIP
;








: ENUM ( n -- n+1 ) DUP 1+ SWAP CONSTANT ;


\ Data structures
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
