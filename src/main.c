#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "program.h"
#include "vm.h"

#define MAX_LINE 1000

#define MAX_PROGRAM 100

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <program_file>\n", argv[0]);
        return 1;
    }

    // Open file
    FILE* file = fopen(argv[1], "r");
    if (!file)
    {
        perror("fopen");
        return 1;
    }

    char lines[MAX_LINE][MAX_LEN];
    size_t line = 0;

    Program vm_program = {0};

    // read each line into the lines array
    while (line < MAX_LINE && fgets(lines[line], MAX_LEN, file))
    {
        lines[line][strcspn(lines[line], "\r\n")] = '\0';
        line++;
    }
    // Close the file stream once all lines have been
    // read.
    fclose(file);

    // pars each line using the parser into a list of instructions
    int pars_res = parse_lines(lines, line, &vm_program);

    if (pars_res == 1)
    {
        fprintf(stderr, "ERROR when parsing code\n");
        return 1;
    }

    VM vm;

    vm_init(&vm);
    VmResult res = vm_run(&vm, &vm_program);

    switch (res)
    {
    case VM_HALTED:
        return 0;
    case VM_EOF:
        fprintf(stderr, "VM finished without HALT\n");
        return 0;
    case VM_ERR_PC_OOB:
        fprintf(stderr, "[ERROR] VM finished with PC out-of-bounds error\n");
        return 1;
    case VM_ERR_INVALID_OPCODE:
        fprintf(stderr, "[ERROR] VM finished with invalid opcode error\n");
        return 1;
    case VM_ERR_INVALID_REGISTER:
        fprintf(stderr, "[ERROR] VM finished with invalid register error\n");
        return 1;
    case VM_ERR_STACK_OVERFLOW:
        fprintf(stderr, "[ERROR] VM finished with stack-overflow error\n");
        return 1;
    case VM_ERR_STACK_UNDERFLOW:
        fprintf(stderr, "[ERROR] VM finished with stack-underflow error\n");
        return 1;
    default:
        fprintf(stderr, "[ERROR] Unknown VM result\n");
        return 1;
    }
}
