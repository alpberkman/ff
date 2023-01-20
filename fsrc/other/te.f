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


WARN Now warn can be made multiline by appending \ at the end of\
a line. The only caveat is that the last character before \ cant\
be \ itself \ \
Just like in the example above a, if you want the last  character\
be a \ just put a space after the backslash


4369 . 	WARN  0x1111
4660 . 	WARN  0x1234
1 . 	WARN  0x0001
123 . 	WARN  0x007B
43981 . WARN  0xABCD


: DUMP DUP DUP . 32 EMIT
	CSZ ADD DUP LDB 31 AND SWAP 1 ADD \ Get the starting address and the length
	SWAP 1 PICKP ADD SWAP \ Calculate the end address
	BEGIN 1 PICKP 1 PICKP GT WHILE DUP LDB EMIT 1 ADD REPEAT \ Print the word
	10 EMIT \ Print space
	DROP DROP \ Remove the addresses
	DUP 0 NEQ IF LDC SELF THEN DROP \ Call itself if it hasn' t reached the end of the list
;
: DUMP LDL DUMP 10 EMIT ;
\ Prints the words along with their addresses


: x deb WARN deb is renamed as x
;













