
#ifndef VM_H_
#define VM_H_
#include "vm.h"
#endif


int fget_input(char *buffer);
void repl(VM *vm);

extern char *script;
int sget_input(char *buffer);
void eval(VM *vm, char *str);

void eval_word(VM *vm, char *word);
void eval_comp(VM *vm, char *word);
void eval_intr(VM *vm, char *word);

ptr find_addr(VM *vm, const char *word);
int isnumber(const char *word);




