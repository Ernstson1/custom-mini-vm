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
    parse_lines(lines, line, &vm_program);

    VM vm;

    vm_init(&vm);
    VmResult res = vm_run(&vm, &vm_program);

    if (res == VM_HALTED)
    {
        printf("Program done\n");
        return 0;
    }
    else if (res == VM_EOF)
    {
        printf("Program done\n");
        return 0;
    }
    else if (res == VM_ERR_INVALID_OPCODE)
    {
        printf("Wrong opcode\n");
        return 1;
    }

    return 1;
}
