#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction.h"

#define MAX_LINE 1000
#define MAX_LEN 50

#define MAX_PROGRAM 100

OpCode get_opcode(const char* token)
{
    if (strcmp(token, "MOV") == 0)
        return OP_MOV;
    if (strcmp(token, "ADD") == 0)
        return OP_ADD;
    if (strcmp(token, "SUB") == 0)
        return OP_SUB;
    if (strcmp(token, "PRINT") == 0)
        return OP_PRINT;
    if (strcmp(token, "HALT") == 0)
        return OP_HALT;
    return OP_UNKNOWN;
}

void parse_lines(char lines[][MAX_LEN], size_t line_count, Instruction* program,
                 size_t* program_size)
{
    for (size_t i = 0; i < line_count; i++)
    {
        char* tokens[4];
        int token_count = 0;
        char* tok = strtok(lines[i], " ,");
        while (tok && token_count < 4)
        {
            tokens[token_count++] = tok;
            tok = strtok(NULL, " ,");
        }

        if (token_count == 0)
            continue; // tom rad

        Instruction instr;
        instr.opcode = get_opcode(tokens[0]);
        instr.a = instr.b = instr.c = 0;

        switch (instr.opcode)
        {
            case OP_MOV:
                // MOV Rn, value
                instr.a = atoi(tokens[1] + 1); // "R1" → 1
                instr.b = atoi(tokens[2]);
                break;

            case OP_ADD:
            case OP_SUB:
                // ADD Rd, Rs1, Rs2
                instr.a = atoi(tokens[1] + 1);
                instr.b = atoi(tokens[2] + 1);
                instr.c = atoi(tokens[3] + 1);
                break;

            case OP_PRINT:
                instr.a = atoi(tokens[1] + 1);
                break;

            case OP_HALT:
            default:
                break;
        }

        program[*program_size] = instr;
        (*program_size)++;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <program_file>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file)
    {
        perror("fopen");
        return 1;
    }

    char lines[MAX_LINE][MAX_LEN];
    size_t line = 0;

    Instruction program[MAX_PROGRAM];
    size_t program_size = 0;

    while (line < MAX_LINE && fgets(lines[line], MAX_LEN, file))
    {
        lines[line][strcspn(lines[line], "\r\n")] = '\0';
        line++;
    }
    // Close the file stream once all lines have been
    // read.
    fclose(file);
    parse_lines(lines, line, program, &program_size);

    return 0;
}
