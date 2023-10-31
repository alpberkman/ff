clear
vocab
' vocab disasm


: BURY R> SWAP R> SWAP R< R< R< ;
: UNCOVER R> R> R> SWAP R< SWAP R< ;

: I 1 RICK ;
: I-LIMIT 2 RICK ;
: I-JMP 3 RICK ;
: J 4 RICK ;
: J-LIMIT 5 RICK ;
: J-JMP 6 RICK ;

\ : I+ R> SWAP UNCOVER + BURY R< ;

: UNLOOP UNCOVER DROP UNCOVER DROP UNCOVER DROP ;

: BRANCH0-UNLOOP
  R> DROP 6 CELLS IP@ + JZ
  R> CELL+ UNLOOP JMP
  R> @ JMP
;


: [DO]
  R> DUP @ R< CELL+ R<
  SWAP BURY BURY
;
: DO
  ['] [DO] ,
  HERE CELL ALLOT
  HERE
; IMMEDIATE

: [LOOP]
  UNCOVER 1+ BURY
  1 RICK 2 RICK = BRANCH0-UNLOOP
;
: LOOP
  ['] [LOOP] ,
  ,
  HERE SWAP !
; IMMEDIATE

: [+LOOP]
  1 RICK SWAP
  UNCOVER + BURY
  2 RICK <
  1 RICK 2 RICK >= AND BRANCH0-UNLOOP
;
: +LOOP
  ['] [+LOOP] ,
  ,
  HERE SWAP !
; IMMEDIATE


: [LEAVE] R> DROP R> DROP R> DROP ;
: LEAVE
  ['] [LEAVE] ,
; IMMEDIATE











: CR LF EMIT ;




: x 0 do i . i 5 = if 1 if leave then then cr loop 123 ;



: x do if do if halt then loop then loop ;






: x 0 do i . cr 3 +loop ;
























