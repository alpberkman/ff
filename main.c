#include <stdio.h>
#include <string.h>
#include "ff.h"



int main() {

    VM vm;
    init(&vm);
    words(&vm);
    run(&vm);
    debug(&vm);
    dump(&vm, "dump.c");


    return 0;
}
