#include <string.h>
#include <stdio.h>

#ifndef UTILS_H_
#define UTILS_H_
#include "utils.h"
#endif

#ifndef OPCODES_H_
#define OPCODES_H_
#include "opcodes.h"
#endif

#ifndef REPL_H_
#define REPL_H_
#include "repl.h"
#endif




// For this to work with strings there should be a case
// where it reads form a const string an move the pointer
// global variable for eval because after reading we need to be able to move

int main() {
/*
	int len = 0;
	char *str = "HELLO-WORLD DEBUG KEY ADEBUG 11111111112222222222333333333344444";
	char buf[32];
	while(sget_word(str, buf))
		printf("%s\n", buf);
    return 0;
*/
    VM vm;
    init_vm(&vm);
	extend(&vm);

	char str[] = "HELLO-WORLD DEBUG 65 66 67 EMIT EMIT EMIT ";
	eval(&vm, str);

	repl(&vm);

    return 0;
}

















