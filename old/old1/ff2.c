#include "ff.h"


#include <stdio.h>

#define X 5

#if X == 5
char *p = "4444";
#else
char *p = "5555";
#endif

int main() {
	puts(p);
	int i = 0;
	switch(i++) {
		default: printf("%i\n", i);
	}
	return 0;
}