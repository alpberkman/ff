

: HERE HP @ ;
: LAST LP @ ;


: OVER 1 PICK ;
: ROT R< SWAP R> SWAP ;
: R@ 1 RICK ;
: NIP SWAP DROP ;



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


: [RECURSE] ;
: RECURSE
  ['] [RECURSE] ,
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

: I+ R> SWAP UNCOVER + BURY R< ;

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
  -1 BURY
; IMMEDIATE

: [LOOP]
  1 I+ ( UNCOVER 1+ BURY)
  1 RICK 2 RICK = BRANCH0-UNLOOP
;
: LOOP
  ['] [LOOP] ,
  ,
  BEGIN 1 RICK -1 <> WHILE
    HERE UNCOVER !
  REPEAT UNCOVER DROP
; IMMEDIATE

: [+LOOP]
  I+
  1 RICK 2 RICK >= BRANCH0-UNLOOP
;
: +LOOP
  ['] [+LOOP] ,
  ,
  BEGIN 1 RICK -1 <> WHILE
    HERE UNCOVER !
  REPEAT UNCOVER DROP
; IMMEDIATE


: [LEAVE] R> UNLOOP R< BRANCH ;
: LEAVE
  ['] [LEAVE] ,
  HERE BURY
  CELL ALLOT
; IMMEDIATE


: [NONAME:] ;
: :NONAME
  HERE
  LAST ,
  LP !
  0 C,
  HERE ['] [NONAME:] , ]
;


: WITHIN ( n1 n2 n3 -- flag )
  OVER OVER < IF
    R< OVER > 0= SWAP R> < AND
  ELSE
     SWAP R< OVER > 0= SWAP R> < AND 0=
  THEN
;
: BETWEEN  ( n min max -- flag ) 1+ WITHIN ;


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
  NIP
;


: CHAR BEGIN KEY DUP ISSPACE WHILE DROP REPEAT ;
: [CHAR] CHAR POSTPONE LITERAL ; IMMEDIATE


: \ KEY LF <> IF SELF THEN ; IMMEDIATE
: ( KEY [CHAR] ) <> IF SELF THEN ; IMMEDIATE
: ?( IF POSTPONE ( THEN ; IMMEDIATE
: ) ; IMMEDIATE
: !( KEY DUP [CHAR] ) <> IF EMIT SELF THEN DROP ; IMMEDIATE


: COUNT ( addr1 -- addr2 n ) DUP CELL+ SWAP @ ;

: TYPE ( addr n -- )
  DUP 0 <= IF DROP DROP EXIT THEN
  0 DO DUP I + C@ EMIT LOOP DROP
;

: ["] ;

: [S"]
  R@ CELL+ R@ @
  R@ @ CELL+ R> + R<
;
: S"
  ['] [S"] ,
  HERE 0 ,
  BEGIN KEY DUP [CHAR] " <> WHILE
    C, DUP ++
  REPEAT DROP DROP
; IMMEDIATE

: [."]
  R@ CELL+ R@ @ TYPE
  R@ @ CELL+ R> + R<
;
: ."
  ['] [."] ,
  HERE 0 ,
  BEGIN KEY DUP [CHAR] " <> WHILE
    C, DUP ++
  REPEAT DROP DROP
  ['] ["] ,
; IMMEDIATE


: MOVE ( addr1 addr2 n -- )
  DUP 0 <= IF DROP DROP DROP EXIT THEN
  0 DO OVER I + C@ OVER I + C! LOOP
  DROP DROP
;



: STRNCMP ( addr1 addr2 n -- n )
  DUP 0 <= IF DROP DROP DROP 0 EXIT THEN
  0 DO
    OVER I + C@ OVER I + C@ - DUP 0<> IF
      NIP NIP UNLOOP EXIT
    THEN DROP
  LOOP DROP DROP 0
;

: STREQ ( addr1 addr2 n -- )
  STRNCMP 0=
;


VARIABLE BASE
: BIN     ( -- )  2 BASE ! ;
: DECIMAL ( -- ) 10 BASE ! ;
: HEX     ( -- ) 16 BASE ! ;


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

























