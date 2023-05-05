


: ISUPPER ( n -- flag )
    DUP [CHAR] A >=
    SWAP [CHAR] Z <=
    AND
;
: ISLOWER ( n -- flag )
    DUP [CHAR] a >=
    SWAP [CHAR] z <=
    AND
;
: ISALPHA ( n -- flag )
    DUP ISUPPER
    SWAP ISLOWER
    OR
;




1   int isalnum(int c)

This function checks whether the passed character is alphanumeric.

3 	int iscntrl(int c)

This function checks whether the passed character is control character.
4 	int isdigit(int c)

This function checks whether the passed character is decimal digit.
5 	int isgraph(int c)

This function checks whether the passed character has graphical representation using locale.

7 	int isprint(int c)

This function checks whether the passed character is printable.
8 	int ispunct(int c)

This function checks whether the passed character is a punctuation character.
9 	int isspace(int c)

This function checks whether the passed character is white-space.

11 	int isxdigit(int c)

This function checks whether the passed character is a hexadecimal digit.

The library also contains two conversion functions that accepts and returns an "int".
Sr.No. 	Function & Description
1 	int tolower(int c)

This function converts uppercase letters to lowercase.
2 	int toupper(int c)

This function converts lowerc
