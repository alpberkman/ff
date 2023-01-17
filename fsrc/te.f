: x 0 lt if -1 else 1 then ;
-1 x deb
0 x deb

: echo key emit self ; ( echo)


: x deb exit deb ;


: fib' 3 pickp 3 pickp eq if exit else swap 1 pickp add push push 1 add pop pop self then ;
: fib dup 3 lt if drop 1 else 2 1 1 fib' push drop drop drop pop then ;
19 fib deb


