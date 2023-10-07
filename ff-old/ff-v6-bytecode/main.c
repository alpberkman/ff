#include <stdio.h>
#include <unistd.h>
#include "ff.h"



int main(/*int argc, char *argv[]*/) {

    VM vm;
    init(&vm);
    words(&vm);
    /*
    char in_path[32];
    sprintf(in_path, "/proc/%i/fd/0", getpid());
    FILE *in = fopen(in_path, "w");
    for(int i = 1; i < argc; ++i) {
    	char buf[1024];
    	FILE *src = fopen(argv[i], "r");
    	
    	if(src == NULL) {
    	    printf("Couldn't open %s\n", argv[i]);
    		continue;
    	}
    	
    	while(fgets(buf, 1023, src) != 0)
    		fputs(buf, in);
    	fclose(src);
    }
    fclose(in);
    //return 0;
    */
    run(&vm);
    debug(&vm);
    dump(&vm, "dump.c");
    save(&vm, "dump.mem");


    return 0;
}
