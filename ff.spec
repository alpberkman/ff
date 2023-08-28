Specification for ff forth interpreter:

Level 1: Minimal Word Set
As long as the required words are available, different implementations should work equally.
The following words are the minimal required amount of words to bootstrap the rest of the system.

Stack:          DUP DROP SWAP R< R> (PUSH POP) P@ R@ (PICK RICK)
Control flow:   JMP JZ
Memory:         @ ! (FETCH STORE) C@ C! (CHAR-FETHC CHAR-STORE) CELL
Logic:          = <> > < (EQ NEQ GT LT) AND OR XOR TRUE FALSE << >> (SHR SHL)
Arithmetic:     + - * / % (ADD SUB MUL DIV MOD)
IO:             KEY EMIT
Special:        STATE@ STATE! P@ P! R@ R! H@ H! L@ L!
Definitions:    : ; IMMEDIATE LIT '

Level 2: Required Word Set
*** some of these words are implementation specific???

: [ FALSE STATE! ; IMMEDIATE
: ] TRUE STATE! ;

: POSTPONE ' , ; IMMEDIATE \ append next word as a function
: LITERAL [ ' LIT DUP , , ] , , ; IMMEDIATE \ append whatever is on the stack as a literal
: ['] ' POSTPONE LITERAL ; IMMEDIATE

Level 3: Required VM and Opcodes

Opcodes:

