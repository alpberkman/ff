

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


