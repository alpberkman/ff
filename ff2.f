

: HERE HP @ ;
: LAST LP @ ;


: CELL+ CELL + ;
: CELLS CELL * ;

: BYTE 1 ;
: BYTE+ BYTE + ;
: BYTES BYTE * ;

: B2C DUP BYTE * CELL / SWAP CELL % + ;
: C2B CELL * BYTE / ;


: 1+ 1 + ;
: 1- 1 - ;


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


: +! SWAP OVER @ + SWAP ! ;
: ALLOT HP +! ;
: , HERE ! CELL ALLOT ;
: C, HERE C! BYTE ALLOT ;


: LITERAL ['] LIT , , ; IMMEDIATE


: [SELF] R> @ JMP ;
: SELF
  ['] [SELF] ,
  LAST CODE ,
; IMMEDIATE


: [IF] 5 CELLS IP@ + JZ R> CELL+ JMP R> @ JMP ;
: IF
  ['] [IF] ,
  HERE
  CELL ALLOT
; IMMEDIATE

: [THEN] ;
: THEN
  ['] [THEN] ,
  HERE SWAP !
; IMMEDIATE

: [ELSE] R> @ JMP ;
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

: [AGAIN] R> @ JMP ;
: AGAIN
  ['] [AGAIN] ,
  ,
; IMMEDIATE

: [UNTIL] 5 CELLS IP@ + JZ R> CELL+ JMP R> @ JMP ;
: UNTIL
  ['] [UNTIL] ,
  ,
; IMMEDIATE

: [WHILE] 5 CELLS IP@ + JZ R> CELL+ JMP R> @ JMP ;
: WHILE
  ['] [WHILE] ,
  HERE
  CELL ALLOT
; IMMEDIATE

: [REPEAT] R> @ JMP ;
: REPEAT
  ['] [REPEAT] ,
  SWAP ,
  HERE SWAP !
; IMMEDIATE


: \ KEY 10 <> IF SELF THEN ; IMMEDIATE
: ( KEY 41 <> IF SELF THEN ; IMMEDIATE
: WARN KEY
	DUP 92 = IF
		KEY DUP 10 = IF
			32 EMIT DROP DROP
		ELSE
			SWAP EMIT EMIT
		THEN
		SELF
	THEN
	DUP EMIT 10 <> IF SELF THEN
; IMMEDIATE


: BL ( -- char ) 32 ;
: HT ( -- char ) 09 ;
: LF ( -- char ) 10 ;
: VT ( -- char ) 11 ;
: FF ( -- char ) 12 ;
: CR ( -- char ) 13 ;


: ISSPACE ( n -- flag )
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


: .
  DROP
  [char] ? emit
  [char] ? emit
  [char] ? emit
;

: N-PRINT ( addr n -- )
  OVER + SWAP
  BEGIN OVER OVER <> WHILE
    DUP @ EMIT
    1+
  REPEAT DROP DROP
;

: WORD-PRINT ( word-addr -- )
  DUP NAME
  SWAP FLAGS LEN?
  N-PRINT
;

: VOCAB ( -- )
  LAST
  BEGIN DUP WHILE
  DUP WORD-PRINT
  BL EMIT
  PREV
  REPEAT LF EMIT DROP
;

: ENTRY-FIND ( addr -- addr )
  LAST
  BEGIN OVER OVER < WHILE
  PREV
  REPEAT SWAP DROP
;
: ENTRY-CODE? ( addr -- f )
  DUP ENTRY-FIND CODE =
;
: ENTRY-END ( addr -- n )
  DUP LAST = IF DROP HERE EXIT THEN
  LAST BEGIN OVER OVER PREV <> WHILE PREV REPEAT
  SWAP DROP
;
: DISASM ( addr -- )
  ENTRY-FIND
  DUP WORD-PRINT LF EMIT
  DUP ENTRY-END SWAP CODE
  BEGIN OVER OVER > WHILE
    DUP @ ENTRY-CODE? IF
      DUP @ ENTRY-FIND WORD-PRINT
    ELSE
      DUP @ .
    THEN BL EMIT CELL+
  REPEAT DROP DROP LF EMIT
;


VARIABLE READ-CTR
0 READ-CTR !

: READ-BUF-SIZE 31 ;
HERE READ-BUF-SIZE ALLOT
: READ-BUF LITERAL ;

: READ-APPEND
  READ-BUF READ-CTR @ + !
  1 READ-CTR +!
;

: READ
  0 READ-CTR !
  CHAR READ-APPEND
  BEGIN
    KEY DUP ISSPACE IF DROP EXIT THEN
    READ-APPEND READ-CTR @ READ-BUF-SIZE =
  UNTIL
;

: BUF-PRINT READ-BUF READ-CTR @ N-PRINT ;

: ?EXIT IF R> DROP EXIT THEN ;

: STRNCMP ( addr1 addr2 n -- n )

;

: ENTRY-HEADER
(
  HERE
  LAST ,
  LP !
  READ-CTR @ C,
)
;
: :
  READ
  HERE
  LAST ,
  LP !
  READ-CTR @ C,
  READ-CTR @ 0
  BEGIN OVER OVER > WHILE
    DUP READ-BUF + @ C,
    1+
  REPEAT DROP DROP ]
;

: ;
  LAST FLAGS VIS MARK
  ['] EXIT ,
;

: CONSTANT : POSTPONE LITERAL POSTPONE ; ;

(

: TEST   10 0 DO  CR ." Hello "  LOOP ;
: [DO] R> R> 5 CELLS IP@ + JZ R> CELL+ JMP R> @ JMP ;
: DO
  LITERAL LITERAL
  ['] [']
)

(
: echo key dup [ key q ] LITERAL = if drop exit else emit self THEN ;
: echo2 begin key dup [ key q ] LITERAL = if drop exit else emit then again ;
: echo3 begin key dup emit [ key q ] LITERAL = until ;

: DUP? DUP IF DUP THEN ;
: TF IF TRUE ELSE FALSE THEN ;


: x begin dup while dup 1- repeat ;
)
: CLEAR 27 EMIT 99 EMIT ;
: RED 27 emit [char] [ emit [char] 4 emit [char] 1 emit [char] m emit ;














dump
clear



: MOVE ( addr1 addr2 u -- ) WARN move doesnt work
  0 BEGIN OVER OVER > WHILE
    4 PICK 1 PICK + @
    4 PICK 2 PICK + !
    1+
  REPEAT DROP DROP DROP DROP
;


(
WARN \
make if else then and other loop constructs relocatable \
: inline \
  postpone ' HERE OVER DUP ENTRY-END SWAP - \
  MOVE \
;

)









