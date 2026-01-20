#include "vm.h"
#include "instruction.h"
#include "program.h"
#include <stdio.h>

void vm_init(VM* vm)
{
    // init all registers to value 0
    for (int i = 0; i < VM_REG_COUNT; i++)
        vm->regs[i] = 0;

    vm->pc = 0;
    vm->zero_flag = 0;
    vm->sp = 0;
}

VmResult vm_run(VM* vm, const Program* program)
{
    // do some stuff

    while (vm->pc < program->program_size)
    {
        // get the instruction
        Instruction inst = program->program[vm->pc];

        switch (inst.opcode)
        {
        case OP_MOV:
            vm->regs[inst.a] = inst.b;
            vm->pc++;
            break;
        case OP_ADD:
            vm->regs[inst.c] = vm->regs[inst.a] + vm->regs[inst.b];
            vm->zero_flag = (inst.c == 0) ? 1 : 0;
            vm->pc++;
            break;
        case OP_SUB:
            vm->regs[inst.c] = vm->regs[inst.a] - vm->regs[inst.b];
            vm->zero_flag = (inst.c == 0) ? 1 : 0;
            vm->pc++;
            break;
        case OP_PRINT:
            printf("Contents of R%i: %d\n", inst.a, vm->regs[inst.a]);
            vm->pc++;
            break;
        case OP_HALT:
            return VM_HALTED;
        default:
            printf("Unknown OpCode: Error at line %lu\n", vm->pc);
            return VM_ERR_INVALID_OPCODE;
        }
    }

    return VM_OK;
}
