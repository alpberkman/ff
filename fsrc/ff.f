
: ideb deb ; immediate



: DUP DUP ; TODO i need to do something 
: DROP DROP ;
: SWAP SWAP ;
: >R POP SWAP PUSH PUSH ;
: R> POP POP SWAP PUSH ;
: PICK PICKP ;
: RICK 1 ADD PICKR ;

: = EQ ;
: != NEQ ;
: < LT ;
: > GT ;

: & AND ;
: | OR ;
: ^ XOR ;
: >> SHR ;
: << SHL ;

: + ADD ;
: - SUB ;
: * MUL ;
: / DIV ;
: % MOD ;

: @ LDC ;
: ! STRC ;
: C@ LDB ;
: C! STRB ;

: [ 0 STRS ;
: ] 1 STRS ;

: HERE LDH ;

: KEY KEY ;
: EMIT EMIT ;



: asdfasdf ;




















