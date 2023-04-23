
: app { LDH STRC LDH LIT 2 0 ADD STRH } ;
: lit { 
: imm { LDL LIT 1 0 ADD DUP LDC LIT 0x40 0 OR SWAP STRC } ;






: STUB ." This is a STUB" CR ;

-1 CONSTANT TRUE 
0 CONSTANT FALSE


\ Nucleus Layer
: ! ( 16b addr --) OP_STR ;
: * ( w1 w2 -- w3) OP_MUL ;
: */ ( n1 n2 n3 -- n4) STUB ;
: */MOD ( n1 n2 n3 -- n4 n5) STUB ;
: + ( w1 w2 -- w3) OP_ADD ;
: +! ( w1 addr --) SWAP OVER @ + SWAP ! ;
: - ( w1 w2 -- w3) OP_SUB ;
: / ( n1 n2 -- n3) OP_DIV ;
: /MOD ( n1 n2 -- n3 n4) OVER OVER / >R MOD R> ; 
: 0< ( n -- flag) 0 < ;
: 0= ( w -- flag) 0 = ;
: 0> ( n -- flag) 0 > ;
: 1+ ( w1 -- w2) 1 + ;
: 1- ( w1 -- w2) 1 - ;
: 2+ ( w1 -- w2) 2 + ;
: 2- ( w1 -- w2) 2 - ;
: 2/ ( n1 -- n2) 2 / ;
: < ( n1 n2 -- flag) OP_LT ;
: = ( w1 w2 -- flag) OP_EQ ;
: > ( n1 n2 -- flag) OP_GT ;
: >R ( 16b --) OP_POP OP_SWAP OP_PUSH OP_PUSH ;
: ?DUP ( 16b -- 16b 16b or 0 -- 0) DUP IF DUP THEN ;
: @ ( addr -- 16b) OP_LD ;
: ABS ( n -- u) DUP 0< IF NEGATE THEN ;
: AND ( 16b1 16b2 -- 16b3) OP_AND ;
: C! ( 16b addr --) OP_STRB ;
: C@ ( addr -- 8b) OP_LDB ;
: CMOVE ( addr1 addr2 u --) DUP 0= IF DROP DROP DROP EXIT THEN 0 DO OVER I + C@ OVER I + C! LOOP DROP DROP ;
: CMOVE> ( addr1 addr2 u --) DUP 0= IF DROP DROP DROP EXIT THEN 1 SWAP DO OVER I + 1- C@ OVER I + 1- C! -1 +LOOP DROP DROP ;
: COUNT
: D+ ( wd1 wd2 -- wd3) STUB ;
: D< ( d1 d2 -- flag) STUB ;
: DEPTH
: DNEGATE ( d1 -- d2) STUB ;
: DROP ( 16b --)
: DUP ( 16b -- 16b 16b) 0 PICK ;
: EXECUTE
: EXIT ( --) OP_RET ;
: FILL
: I ( -- w) 0 RPICK ;
: J ( -- w) 2 RPICK ;
: MAX ( n1 n2 -- n3) OVER OVER < IF SWAP THEN DROP ;
: MIN ( n1 n2 -- n3) OVER OVER > IF SWAP THEN DROP ;
: MOD ( n1 n2 -- n3) OP_MOD ;
: NEGATE ( n1 -- n2) NOT 1+ ;
: NOT ( 16b1 -- 16b2) -1 XOR ;
: OR ( 16b1 16b2 -- 16b3) OP_OR ;
: OVER ( 16b1 16b2 -- 16b1 16b2 16b3) 1 PICK ;
: PICK ( +n -- 16b) OP_PICK ;
: R> ( -- 16b) OP_POP OP_POP OP_SWAP OP_PUSH ;
: R@ ( -- 16b) R> DUP R> ;
: ROLL
: ROT ( 16b1 16b2 16b3 -- 16b2 16b3 16b1) >R SWAP <R SWAP ;
: SWAP ( 16b1 16b2 -- 16b2 16b1) OP_SWAP ;
: U< (  u1 u2 -- flag) STUB ;
: UM* ( u1 u2 -- ud) * ;
: UM/MOD ( ud u1 -- u2 u3) STUB ;
: XOR ( 16b1 16b2 -- 16b3) OP_XOR ;


\ Device Layer 
: BLOCK ( u -- addr) STUB ;
: BUFFER ( u -- addr) STUB ; 
: CR ( --) 13 EMIT 10 EMIT ;
: EMIT ( 16b --) OP_PUT ;
EXPECT
: FLUSH ( --) STUB ;
: KEY ( -- 16b) OP_GET ;
: SAVE-BUFFERS ( --) STUB ;
: SPACE ( --) 32 EMIT ;
: SPACES ( +n --) DUP 0= IF DROP EXIT THEN 0 DO SPACE LOOP ;
: TYPE ( addr +n --) DUP 0= IF DROP DROP EXIT THEN 0 DO DUP I + C@ EMIT LOOP DROP ;
: UPDATE ( --) STUB ;


\ Interpreter Layer 
#  #>  #S  
( ) #TIB TIB
  '  ( delete this) -TRAILING  .  .(  <#  >BODY  >IN  
ABORT  BASE 
: BLK ( -- addr) STUB ;
  CONVERT  DECIMAL  DEFINITIONS  FIND  
FORGET  FORTH  FORTH-83  
: HERE ( -- addr) OP_LDH ;
 HOLD  LOAD  PAD  QUIT  SIGN 
SPAN  

( ) 80 ALLOT
( ) CONSTANT TIB
U.  WORD

\ Compiler Layer 
+LOOP  ,  ."  :  ;  
: ABORT" STUB ;
: ALLOT ( w --) HERE +! ;
  BEGIN  COMPILE  CONSTANT  
CREATE  DO  DOES>  ELSE  IF  IMMEDIATE  LEAVE  LITERAL  LOOP 
REPEAT  STATE  THEN  UNTIL  VARIABLE  VOCABULARY  WHILE    
[']  [COMPILE]  ]
















































