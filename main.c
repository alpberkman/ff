#include <stdio.h>
#include "ff.h"

int main() {
	puts("Hello World");
	
	VM vm;
	init(&vm);
	vm.psp = 3;
	vm.rsp = 5;
	vm.hp = 13;
	debug(&vm);
	
	int arr[2] = {123, 666};
	int i = 1;
	//arr[i++] = arr[i-1];
	//printf("%i %i %i\n", i, arr[0], arr[1]);
	
	i = 1;
	int x = arr[--i];
	printf("%i %i\n", i, x);
	
	signed char a = -123;
	unsigned char b = a;
	printf("%i\n", b);
	
	return 0;
}
