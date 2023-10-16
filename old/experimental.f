: DIVISIBLE ( u1 u2 -- u1 u2 f ) OVER OVER % 0 = ;

: TRAVERSE ( addr -- )
  ' SWAP
  BEGIN DUP 0 <> WHILE
    OVER CALL
    PREV
  REPEAT DROP DROP
;


last .header .link
last .header .flags
\ wrong
: .header ;
: .link @ ;
: .flags CELL + C@ ;

: IMMEDIATE LAST @ CELL + DUP C@ 1 6 << OR SWAP C! ;

: OVER 1 PICK ;


: BURY R> SWAP R> SWAP R< R< R< ;
: UNCOVER R> R> R> SWAP R< SWAP R< ;

: I 1 RICK ;
: J 5 RICK ;
: I-LIMIT 3 RICK ;

: I+ R> SWAP R> SWAP R> + R< R< R< ;

: UNLOOP R> R> UNCOVER DROP UNCOVER DROP R< R< ;

: [DO] SWAP BURY BURY ;
: DO
  ['] [DO] ,
  HERE
; IMMEDIATE

: [LOOP]
  1 I+
  1 RICK 2 RICK > BRANCH0
  halt
  UNLOOP
;
: LOOP
  ['] [LOOP] ,
  ,
; IMMEDIATE
: x 333 222 do halt loop ;
