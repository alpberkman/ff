#include <stdio.h>
#include <stdlib.h>
#include "ff.h"
#include "setup.h"

#define pint(X) printf("%i\n", X)


int main(/*int argc, char *argv[]*/) {
    VM X;
    init(&X);
    list(&X);
    stacks(&X);
    exec(&X, ADD);
    stacks(&X);
    dump(&X, "ff.dump");
    carr(&X, "rom.h");

    return 0;
}
