clear
vocab
' vocab disasm


: y 3 0 do i loop ;
: x 333 330 do i unloop exit loop ;
: z 3 0 do i 2 +loop ;
\ x y
z


0 psp! 1 rsp! clear
: x 10 0 do i . LF emit i 7 = if leave then loop ;
