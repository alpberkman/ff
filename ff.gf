// Ass
LIT : POP, DUP, CSZ, ADD, PUSH, LDC, RET


// Premade
: HERE@ HERE @ ;
: HERE! HERE ! ;

: LAST@ LAST @ ;
: LAST! LAST ! ;

: STATE@ STATE @ ;
: STATE! STATE ! ;

// Main
: [ 0 STATE! ; IMMEDIATE
: ] 1 STATE! ;

: TRUE -1 ;
: FALSE 0 ;

: CELL+ CELL + ;
: CELLS CELL * ;

: , HERE@ ! HERE@ CELL+ HERE! ;
: C, HERE@ C! HERE@ 1 + HERE! ;

: ALLOT CELLS HERE@ + HERE! ;

: LITERAL ['] LIT , , ; IMMEDIATE

: 1+! ;
: CELL+! ;
