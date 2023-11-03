

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


warn my : and ; are not used because of the outer interpreter


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



: HERE HP @ ;
: LAST LP @ ;


: (( ['] ( EXECUTE ;
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









WARN how about <> for appended functions


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




