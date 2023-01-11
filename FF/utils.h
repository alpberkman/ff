
#ifndef VM_H_
#define VM_H_
#include "vm.h"
#endif


void init_vm(VM *vm);
void save_vm(VM *vm, const char *image);
void load_vm(VM *vm, const char *image);

void make_app_from_img(const char *image);
void make_app_from_src(const char *source);

void extend_dict(VM *vm, char *name, byte flags, byte *fun, ptr len);
void extend_cffi(VM *vm, char *name, byte flags, vm_fun fun);

void debug(VM *vm);

void extend(VM *vm);




