: x 0 lt if -1 else 1 then ;
-1 x deb
0 x deb

: echo key emit self ; ( echo)


: x deb exit deb ;


: fib' 3 pickp 3 pickp eq if exit else swap 1 pickp add push push 1 add pop pop self then ;
: fib dup 3 lt if drop 1 else 2 1 1 fib' push drop drop drop pop then ;
19 fib deb


: x begin dup 1 add dup 10 eq if exit then again ;
0 x deb

: y begin dup 1 add dup 10 eq until ;
0 y deb

: z begin dup 10 lt while dup 1 add repeat ;
0 z deb

