#include <stdio.h>
#include <string.h>
#include "ff.h"

void test(unsigned char *arr, int size) {
    VM vm;
    init(&vm);
    memcpy(vm.mem, arr, size);
    vm.hp = size;
    run(&vm);
    debug(&vm);
}

void testall(void) {

    unsigned char arr1[] = {
        LIT,1,0,LIT,5,0,
        DUP,MUL,ADD,
        DUP,NOP,NOP,NOP,
        LIT,5,0,ADD,SWAP,DROP,
        DUP,PUSH,
        LIT,-1,-1,MUL,
        POP,
        PUSH,
        LIT,0,0,PICKP,
        LIT,0,0,PICKR,
        LIT,1,0,PICKP,
        HALT
    };
    test(arr1, sizeof(arr1));

    unsigned char arr2[] = {
        LIT,0,0,LIT,-1,-1,XOR,
        LIT,1,0,AND,DUP,
        LIT,2,0,OR,
        LIT,1,0,SHR,
        LIT,3,0,SHL,
        ADD,
        LIT,5,0,SUB,
        LIT,3,0,MUL,
        DUP,
        LIT,7,0,DIV,
        SWAP,
        LIT,7,0,MOD,
        LIT,0,0,LIT,1,0,EQ,
        LIT,0,0,LIT,0,0,EQ,
        LIT,0,0,LIT,1,0,NEQ,
        LIT,0,0,LIT,0,0,NEQ,
        LIT,0,0,LIT,1,0,LT,
        LIT,0,0,LIT,1,0,SWAP,LT,
        LIT,0,0,LIT,1,0,GT,
        LIT,0,0,LIT,1,0,SWAP,GT,
        HALT
    };
    test(arr2, sizeof(arr2));

    unsigned char arr3[] = {
        LIT,1,0,
        LIT,10,0,JZ,
        LIT,3,3,
        LIT,3,3,
        LIT,3,3,
        HALT
    };
    test(arr3, sizeof(arr3));

    unsigned char arr4[] = {
        LIT,7,0,CALL,
        LIT,3,3,
        LIT,3,3,
        LIT,3,3,
        
        LIT,21,0,PUSH,RET,
        LIT,3,3,
        LIT,3,3,
        LIT,3,3,
        
        HALT
    };
    test(arr4, sizeof(arr4));

    unsigned char arr5[] = {
        NOP,NOP,NOP,NOP,
        LDI,DUP,DUP,
        LIT,0,0,STRB,
        LIT,1,0,STRB,
        LIT,2,0,STRC,
        HALT
    };
    test(arr5, sizeof(arr5));
}

int main() {

    testall();


    return 0;
}
