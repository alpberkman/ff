clear
vocab
' vocab disasm

: DIVISIBLE ( u1 u2 -- u1 u2 f ) OVER OVER % 0 = ;

: TRAVERSE ( addr -- )
  ' SWAP
  BEGIN DUP 0 <> WHILE
    OVER CALL
    PREV
  REPEAT DROP DROP
;

: (( ['] ( CALL ;
here
here
((

3 type of binaries:
    - C interpreter: C outer interpreter + basic binary words
                or everything in C
                or C outer interpreter + basic binary words + macro words
    - just vm: vm.o that you link with your binary vm opcodes
                or vm.elf that reads your binary and saves it to it's memory
                or vm.elf that takes a deciphered source code -> for security
                or vm.elf that reads source code and evaluates it


have a make-header word

Use this to save comments and add it to disasm to print the comments as well



maybe have a word called traverse that traverses a single liked list and calls a function


last traverse print-header
)
here swap - n-print

WARN change n-print to type ( addr u -- )
WARN good variable usuage for ff_interp VARIABLE rnd   HERE rnd !
warn ." doesnt work in runtime


: RED
27 emit [char] [ emit
[char] 4 emit [char] 1 emit
[char] m emit ;



: ESC 27 EMIT ;
: ESC[ ESC [CHAR] [ EMIT ;
: M [CHAR] m emit ;

: .  dup 10 / [char] 0 + emit 10 % [char] 0 + emit ;

: RESET ESC[ 0 .  M ;
: BOLD ESC[ 1 . M ;
: FAINT ESC[ 2 . M ;
: ITALIC ESC[ 3 . M ;
: UNDERLINE ESC[ 4 . M ;


0 constant black
1 constant red
2 constant green
3 constant yellow
4 constant blue
5 constant magenta
6 constant cyan
7 constant white


: red 1 ;
: green 2 ;

: FG ESC[ 30 + . M ;
: BG ESC[ 40 + . M ;






WARN how about <> for appended functions

: NOT IF FFALSE ELSE TRUE THEN ;


\ normal word in runtime, macro in compile
: [RUNTIME>] STATE @ NOT BRANCH0 ;
: RUNTIME>
  ['] [RUNTIME>] ,
  HERE CELL ALLOT
; IMMEDIATE

: [COMPILE>] R> DROP ;
: COMPILE>
  ['] [COMPILE>] ,
  HERE SWAP !
  IMMEDIATE
; IMMEDIATE



: x
  RUNTIME> 5
  COMPILE> 3 POSTPONE LITERAL
;
: y x ;
x y
drop drop



\ normal word in runtime, a different word in compile
: [RUNTIME>] STATE @ NOT BRANCH0 ;
: RUNTIME>
  ['] [RUNTIME>] ,
  HERE CELL ALLOT
; IMMEDIATE

: [COMPILE>] R> , ;
: COMPILE>
  ['] [;] ,
  HERE SWAP !
  ['] [COMPILE>] ,
  IMMEDIATE
; IMMEDIATE



: x
  RUNTIME> 5
  COMPILE> 3
;
: y x ;
x y

(
Both can be used to prevent macros to be used in interpreter
First one can compile the macros
the second one can be the macros
)










