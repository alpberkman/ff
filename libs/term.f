: RED
27 emit [char] [ emit
[char] 4 emit [char] 1 emit
[char] m emit ;

: .  dup 10 / [char] 0 + emit 10 % [char] 0 + emit ;


: ESC 27 EMIT ;
: ESC[ ESC [CHAR] [ EMIT ;
: M [CHAR] m emit ;


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
