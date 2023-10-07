


: B2C DUP BYTE * CELL / SWAP CELL % + ;
: C2B CELL * BYTE / ;

: R>> R> R> ;
: R<< R< R< ;

: [(] BRANCH ;
: (
  ['] [(] ,
  [CHAR] ( C,
  HERE CELL ALLOT
  BEGIN
  KEY DUP C,
  [CHAR] ) = UNTIL
  HERE !
; IMMEDIATE
