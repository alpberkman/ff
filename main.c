#include <stdio.h>
#include "ff.h"



int main(/*int argc, char *argv[]*/) {

    VM vm;
    init(&vm);
    //words(&vm);
    debug(&vm);

	*((cell *) &(vm.mem[vm.hp])) = 120;
	vm.hp += sizeof(cell);
	*((cell *) &(vm.mem[120])) = 240;
	*((cell *) &(vm.mem[240])) = RET;
	debug(&vm);

    cycle(&vm);debug(&vm);
    cycle(&vm);debug(&vm);
    cycle(&vm);debug(&vm);
    cycle(&vm);debug(&vm);
    dump(&vm, "dump.c");
    save(&vm, "dump.mem");
printf("%10i\n", ADD);
printf("%10i\n", SUB);
    return 0;
}
