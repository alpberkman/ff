
: DUMP DUP
	CSZ ADD DUP LDB 31 AND SWAP 1 ADD \ Get the starting address and the length
	SWAP 1 PICKP ADD SWAP \ Calculate the end address
	BEGIN 1 PICKP 1 PICKP GT WHILE DUP LDB EMIT 1 ADD REPEAT \ Print the word
	32 EMIT \ Print space
	DROP DROP \ Remove the addresses
	DUP 0 NEQ IF LDC SELF THEN DROP \ Call itself if it hasn' t reached the end of the list
;
: DUMP LDL DUMP 10 EMIT ;



: ideb deb ; immediate


WARN Welcome to the Playground
dump cr




: x deb WARN deb is renamed as x
;





