#include "vm.h"
#include "instruction.h"
#include "program.h"
#include <stddef.h>
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

int check_register(const int reg)
{
    return (reg >= 0 && reg < VM_REG_COUNT);
}

VmResult vm_run(VM* vm, const Program* program)
{
    // do some stuff

    while (vm->pc < program->program_size)
    {
        if (vm->pc > program->program_size)
            return VM_ERR_PC_OOB;

        // get the instruction
        const Instruction* inst = &program->program[vm->pc];
        vm->fetch_pc = vm->pc;
        vm->pc++;
        int result;

        switch (inst->opcode)
        {
        case OP_MOV:
            if (!check_register(inst->a))
            {
                return VM_ERR_INVALID_REGISTER;
            }
            vm->regs[inst->a] = inst->b;
            break;

        case OP_ADD:
            if (!check_register(inst->a))
            {
                return VM_ERR_INVALID_REGISTER;
            }
            if (!check_register(inst->b))
            {
                return VM_ERR_INVALID_REGISTER;
            }
            if (!check_register(inst->c))
            {
                return VM_ERR_INVALID_REGISTER;
            }
            result = vm->regs[inst->a] + vm->regs[inst->b];
            vm->zero_flag = (result == 0) ? 1 : 0;
            vm->regs[inst->c] = result;
            break;

        case OP_SUB:
            if (!check_register(inst->a))
            {
                return VM_ERR_INVALID_REGISTER;
            }
            if (!check_register(inst->b))
            {
                return VM_ERR_INVALID_REGISTER;
            }
            if (!check_register(inst->c))
            {
                return VM_ERR_INVALID_REGISTER;
            }

            result = vm->regs[inst->a] - vm->regs[inst->b];
            vm->zero_flag = (result == 0) ? 1 : 0;
            vm->regs[inst->c] = result;
            break;

        case OP_PRINT:
            if (!check_register(inst->a))
            {
                return VM_ERR_INVALID_REGISTER;
            }

            printf("Contents of R%i: %d\n", inst->a, vm->regs[inst->a]);
            break;

        case OP_HALT:
            return VM_HALTED;

        default:
            return VM_ERR_INVALID_OPCODE;
        }
    }

    if(vm->pc == program->program_size) return VM_EOF;
    return VM_ERR_PC_OOB;
}
