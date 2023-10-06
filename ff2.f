

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





: .
  DROP
  [char] ? emit
  [char] ? emit
  [char] ? emit
;

: N-PRINT ( addr n -- )
  OVER + SWAP
  BEGIN OVER OVER > WHILE
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
  ['] [;] ,
; IMMEDIATE

: [CONSTANT] ;
: CONSTANT
  :
  ['] [CONSTANT] ,
  POSTPONE LITERAL POSTPONE ;
;

(

when using constant instead of tagining it both with
['] and [constant] have a word that compiles:
header - name - [CONSTANT] - ???

: [CONSTANT] R> @ ;
: CONSTANT
  MAKE-HEADER [ IMMEDIATE VISIBLE ]
  ['] CONSTANT ,
  POSTPONE LITERAL
;

bin2src

same for variable

)


: CLEAR 27 EMIT 99 EMIT ;
: RED 27 emit [char] [ emit [char] 4 emit [char] 1 emit [char] m emit ;




















