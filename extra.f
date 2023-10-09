


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

