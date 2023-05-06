#include <stdio.h>
#include <stdlib.h>
#include "ff.h"

#define pint(X) printf("%i\n", X)

VM vm;

void backup(void) {
    dump(&vm, "dump.c");
    save(&vm, "dump.mem");
}


int main(/*int argc, char *argv[]*/) {

    atexit(backup);

    init(&vm);
    words(&vm);
    debug_words2(&vm);

    for(;;) {
        interp(&vm);
        run(&vm);
    }

    return 0;
}
