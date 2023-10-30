
: ENUM ( -- )
  CREATE 0 ,
  DOES> DUP @ DUP 1+ ROT ! CONSTANT
;

enum x
x x1
x x2
cr x1 .
cr x2 .
cr

: CONSTANT
  CREATE ,
  DOES> @
;
123 CONSTANT x
cr cr cr
see CONSTANT cr cr
see x cr cr
