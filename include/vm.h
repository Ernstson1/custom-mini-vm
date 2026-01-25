#ifndef VM_H
#define VM_H
#include "program.h"
#include <stdint.h>

#define VM_REG_COUNT 8

#define VM_STACK_SIZE 256

typedef struct{
    int32_t regs[VM_REG_COUNT];
    size_t pc;
    int zero_flag;

    int32_t stack[VM_STACK_SIZE];
    size_t sp;
} VM;


typedef enum {
    VM_HALTED,
    VM_ERR_PC_OOB,
    VM_ERR_INVALID_OPCODE,
    VM_ERR_INVALID_REGISTER,
    VM_ERR_STACK_OVERFLOW,
    VM_ERR_STACK_UNDERFLOW,
    VM_EOF
} VmResult;

void vm_init(VM* vm);
VmResult vm_run(VM* vm, const Program* program);

#endif // VM_H
