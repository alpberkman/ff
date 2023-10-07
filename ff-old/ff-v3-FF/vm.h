
typedef short cell;
typedef char byte;
typedef unsigned short ptr;

#define CELL_SIZE sizeof(cell)
#define BYTE_SIZE sizeof(byte)


#define FALSE 0
#define TRUE -1


#define PSTACK_SIZE (0xff)
#define RSTACK_SIZE (0xff)
#define MEMORY_SIZE (0xffff)


#define MAX_LEN 0x1f
#define MASK_VIS 0x80
#define MASK_IMM 0x40
#define MASK_LEN 0x1F


typedef enum VM_STATUS VM_STATUS;
enum VM_STATUS {
    DEAD,
    ALIVE,
};

typedef enum VM_MODE VM_MODE;
enum VM_MODE {
    INTERPRET,
    COMPILE,
};

typedef enum VM_ERROR VM_ERROR;
enum VM_ERROR {
    NO_ERROR, END_OF_MEM,
	UNKNOWN_OPCODE, UNKNOWN_WORD,
    RS_UNDERFLOW, RS_OVERFLOW,
    PS_UNDERFLOW, PS_OVERFLOW,
    DIV_BY_ZERO,
    //STORE_ERROR, FETCH_ERROR,
    //INTERPRETER_ERROR, COMPILER_ERROR,
};


typedef struct VM VM;
struct VM {
    VM_STATUS status;
    ptr i_ptr;		    //Instruction pointer

    VM_MODE mode;
    VM_ERROR error;

    cell ps[PSTACK_SIZE];
    ptr ps_ptr;

    cell rs[RSTACK_SIZE];
    ptr rs_ptr;

    byte mem[MEMORY_SIZE];
    ptr mem_ptr;		//Pointer to last empty memory address
    ptr wrd_ptr;		//Pointer to last word

	cell reg_a;
	cell reg_b;
};

typedef void (*vm_fun)(VM *vm);
#define VM_FUN_SIZE sizeof(vm_fun)

VM_ERROR run_vm(VM *vm);
byte get_opcode(VM *vm);

byte get_mem(VM *vm, cell addr);
void set_mem(VM *vm, cell addr, cell c);

void ppush(VM *vm, cell c);
cell ppop(VM *vm);
void rpush(VM *vm, cell c);
cell rpop(VM *vm);










