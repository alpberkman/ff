#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char in_path[32];
    char buf[1024];
    FILE *in;

    sprintf(in_path, "/proc/%i/fd/0", getpid());
    //sprintf(in_path, "test.txt");

    in = fopen(in_path, "w");
    for(int i = 1; i < argc; ++i) {
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

    sleep(5);

    /*
        in = fopen(in_path, "r");
        while(fgets(buf, 1023, in) != 0)
            puts(buf);
        fclose(in);
    */
    int c;
    while((c=getchar()) != EOF)
        putchar(c);
    return 0;
}
