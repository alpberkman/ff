

: CELL+ CELL + ;
: CELL- CELL - ;
: CELLS CELL * ;

: R@ 0 RICK ;


: TRUE -1 ;
: FALSE 0 ;


: VIS 1 7 LSHIFT ;
: IMM 1 6 LSHIFT ;
: LEN 31 ;

: IMMEDIATE LAST CELL+ DUP C@ IMM OR SWAP C! ;
: VISIBLE  LAST CELL+ DUP C@ VIS OR SWAP C! ; IMMEDIATE


: [ FALSE STATE! ; IMMEDIATE
: ] TRUE STATE! ;


: , HERE ! HERE CELL+ HERE! ;
: LITERAL [ ' LIT , ] [ ' LIT , ] , , ; IMMEDIATE


: [SELF] R> @ JMP ;
: SELF
    [ ' [SELF] ] LITERAL ,
    LAST CELL+ DUP C@ LEN AND + 1 + ,
; IMMEDIATE




: [IF]
    DEBs 6 CELLS IP + JZ
    DEBs R> CELL+ JMP
    DEBs R> DROP R> @ JMP
;
: IF
    [ ' [IF] ] LITERAL ,
    HERE HERE CELL+ HERE!
; IMMEDIATE


: IF
    [ ' LIT ] LITERAL ,
    HERE HERE CELL+ HERE!
    -32757 ,
; IMMEDIATE


: [THEN] ;
: THEN
    [ ' [THEN] ] LITERAL ,
    HERE SWAP !
; IMMEDIATE


: x if 2 then ;




: ECHO KEY EMIT SELF ;








: [IF] HERE HERE CELL 3 * + HERE! ;
: IF [IF] ; IMMEDIATE
: [THEN]
    DUP [ ' LIT ] LITERAL SWAP ! CELL+
    DUP HERE SWAP ! CELL+
    [ ' JMP ] LITERAL SWAP !
;
: THEN [THEN] ; IMMEDIATE

: x if 1 then ;









: SELF
    [ ' LIT ] LITERAL ,
    LAST CELL+ DUP C@ LEN AND + 1 + ,
    [ ' JMP ] LITERAL ,
; IMMEDIATE

: [IF] HERE HERE CELL 3 * + HERE! ;
: IF [IF] ; IMMEDIATE
: [THEN]
    DUP [ ' LIT ] LITERAL SWAP ! CELL+
    DUP HERE SWAP ! CELL+
    [ ' JMP ] LITERAL SWAP !
;
: THEN [THEN] ; IMMEDIATE

: x if 1 then ;






: ECHO KEY EMIT SELF ;


: \ KEY 10 <> IF SELF THEN ; IMMEDIATE


: [IF]
: if      ' jmp0 , here 999 , ; immediate
