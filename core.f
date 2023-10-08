

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


: INVERT -1 XOR ;
: 0= 0 = ;
: 0< 0 < ;
: 0> 0 > ;
: 0<> 0 <> ;

: >= < 0= ;
: <= > 0= ;


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

: BRANCH0-UNLOOP
  R> DROP 6 CELLS IP@ + JZ
  R> CELL+ UNLOOP JMP
  R> @ JMP
;


: [DO] SWAP BURY BURY ;
: DO
  ['] [DO] ,
  HERE
; IMMEDIATE

: [LOOP]
  1 I+
  1 RICK 2 RICK = BRANCH0-UNLOOP
;
: LOOP
  ['] [LOOP] ,
  ,
  BEGIN 1 RICK -1 = WHILE
    UNCOVER DROP
    HERE UNCOVER !
  REPEAT
; IMMEDIATE

: [+LOOP]
  I+
  1 RICK 2 RICK >= BRANCH0-UNLOOP
;
: +LOOP
  ['] [+LOOP] ,
  ,
  BEGIN 1 RICK -1 = WHILE
    UNCOVER DROP
    HERE UNCOVER !
  REPEAT
; IMMEDIATE


: [LEAVE] R> UNLOOP R< BRANCH ;
: LEAVE
  ['] [LEAVE] ,
  HERE BURY -1 BURY
  CELL ALLOT
; IMMEDIATE


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
  DUP 0= IF DROP DROP EXIT THEN
  0 DO DUP @ EMIT 1+ LOOP DROP
;

: [."]
  R> DUP CELL+ OVER @ TYPE
  DUP @ + CELL+ R<
;
: ["] ;
: ."
  ['] [."] ,
  HERE 0 ,
  BEGIN KEY DUP [CHAR] " <> WHILE
    C, DUP ++
  REPEAT DROP DROP
  ['] ["] ,
; IMMEDIATE











