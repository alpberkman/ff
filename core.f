

: HERE HP @ ;
: LAST LP @ ;


: OVER 1 PICK ;
: ROT R< SWAP R> SWAP ;


: CELL+ CELL + ;
: CELLS CELL * ;

: BYTE+ BYTE + ;
: BYTES BYTE * ;


: +! SWAP OVER @ + SWAP ! ;
: ALLOT HP +! ;
: , HERE ! CELL ALLOT ;
: C, HERE C! BYTE ALLOT ;


: 1+ 1 + ;
: 1- 1 - ;


: NOT 0 = ;


: ++ DUP @ 1+ SWAP ! ;
: -- DUP @ 1- SWAP ! ;



: IMM 1 6 << ;
: VIS 1 7 << ;
: LEN 31 ;

: IMM? C@ IMM AND ;
: VIS? C@ VIS AND ;
: LEN? C@ LEN AND ;

: LINK ;
: FLAGS CELL+ ;
: NAME CELL+ BYTE+ ;
: CODE DUP FLAGS LEN? SWAP NAME + ;
: PREV @ ;

: MARK OVER C@ OR SWAP C! ;
: IMMEDIATE LAST FLAGS IMM MARK ;
: VISIBLE LAST FLAGS VIS MARK ; IMMEDIATE


: [ FALSE STATE ! ; IMMEDIATE
: ] TRUE STATE ! ;

: LITERAL ['] LIT , , ; IMMEDIATE



: BRANCH R> DROP R> @ JMP ;
: BRANCH0 R> DROP 5 CELLS IP@ + JZ R> CELL+ JMP R> @ JMP ;
( : BRANCH0-N
  SWAP
  R> DROP
  7 CELLS IP@ + JZ
  R> CELLS + CELL+ JMP
  R> CELLS + @ JMP
;)


: [SELF] BRANCH ;
: SELF
  ['] [SELF] ,
  LAST CODE ,
; IMMEDIATE


: [IF] BRANCH0 ;
: IF
  ['] [IF] ,
  HERE CELL ALLOT
; IMMEDIATE

: [THEN] ;
: THEN
  ['] [THEN] ,
  HERE SWAP !
; IMMEDIATE

: [ELSE] BRANCH ;
: ELSE
  ['] [ELSE] ,
  HERE SWAP
  CELL ALLOT
  HERE SWAP !
; IMMEDIATE


: [BEGIN] ;
: BEGIN
  ['] [BEGIN] ,
  HERE
; IMMEDIATE

: [AGAIN] BRANCH ;
: AGAIN
  ['] [AGAIN] ,
  ,
; IMMEDIATE

: [UNTIL] BRANCH0 ;
: UNTIL
  ['] [UNTIL] ,
  ,
; IMMEDIATE

: [WHILE] BRANCH0 ;
: WHILE
  ['] [WHILE] ,
  HERE CELL ALLOT
; IMMEDIATE

: [REPEAT] BRANCH ;
: REPEAT
  ['] [REPEAT] ,
  SWAP ,
  HERE SWAP !
; IMMEDIATE


: BURY R> SWAP R> SWAP R< R< R< ;
: UNCOVER R> R> R> SWAP R< SWAP R< ;

: I 1 RICK ;
: J 3 RICK ;
: I-LIMIT 2 RICK ;
: J-LIMIT 4 RICK ;

: I+ R> SWAP R> SWAP R> + R< R< R< ;

: UNLOOP UNCOVER DROP UNCOVER DROP ;

: [DO] SWAP BURY BURY ;
: DO
  ['] [DO] ,
  HERE
; IMMEDIATE

: [LOOP]
  1 I+
  1 RICK 2 RICK = BRANCH0
  ( UNLOOP halt)
;
: LOOP
  ['] [LOOP] ,
  here swap , @
  ['] UNLOOP ,
; IMMEDIATE

( : x 3 0 do 3 0 do I-limit j * i + loop loop ;
)
: y 3 0 do i loop ;

: LEAVE ;
: CONT ;

: [+LOOP] ;
: +LOOP ;


: [FOR] ;
: FOR ;

: [NEXT] ;
: NEXT ;


: [NONAME:] ;
: :NONAME
  HERE
  LAST ,
  LP !
  0 C,
  HERE ['] [NONAME:] , ]
;


: \ KEY 10 <> IF SELF THEN ; IMMEDIATE
: ( KEY 41 <> IF SELF THEN ; IMMEDIATE


: BL ( -- char ) 32 ;
: HT ( -- char ) 09 ;
: LF ( -- char ) 10 ;
: VT ( -- char ) 11 ;
: FF ( -- char ) 12 ;
: CR ( -- char ) 13 ;


: ISSPACE ( n -- )
  0
  OVER BL = OR
  OVER HT = OR
  OVER LF = OR
  OVER VT = OR
  OVER FF = OR
  OVER CR = OR
  SWAP DROP
;


: CHAR BEGIN KEY DUP ISSPACE WHILE DROP REPEAT ;
: [CHAR] CHAR POSTPONE LITERAL ; IMMEDIATE



: TYPE ( addr u -- )
  OVER + SWAP
  BEGIN OVER OVER > WHILE
    DUP @ EMIT
    1+
  REPEAT DROP DROP
;

: [."]
  R> DUP CELL+ OVER @ TYPE
  DUP @ + CELL+ R<
;
: ["] ;
: ."
  ['] [."] ,
  HERE CELL ALLOT 0
  BEGIN KEY DUP [CHAR] " <> WHILE
    C, 1+
  REPEAT DROP SWAP !
  ['] ["] ,
; IMMEDIATE











