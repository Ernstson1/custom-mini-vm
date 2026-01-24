#include "parser.h"
#include <errno.h>

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction.h"
#include "program.h"

#define MAX_LEN 50
#define MAX_REG_COUNT 8

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

int parse_int(const char* s, int* out)
{
    char* end;
    long val;

    errno = 0;
    val = strtol(s, &end, 10);

    if (errno == ERANGE || val < INT_MIN || val > INT_MAX)
        return 0;

    if (end == s || *end != '\0')
        return 0;

    *out = (int)val;

    return 1;
}


int parse_register(const char* s, int* out)
{
    if (!s || s[0] != 'R')
        return 0;

    char buf[16];
    strncpy(buf, s, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    size_t len = strlen(buf);
    if (len < 2)
        return 0;

    // ta bort valfritt trailing comma
    if (buf[len - 1] == ',')
    {
        buf[len - 1] = '\0';
        len--;
        if (len < 2)   // "R," är ogiltigt
            return 0;
    }

    if (!parse_int(buf + 1, out))
        return 0;

    if (*out < 0 || *out >= MAX_REG_COUNT)
        return 0;

    return 1;
}

int parse_register_comma_required(const char* s, int* out)
{
    size_t n = s ? strlen(s) : 0;
    if (n < 2 || s[n - 1] != ',')
        return 0;
    return parse_register(s, out); // parse_register ovan accepterar komma
}

int parse_lines(char lines[][MAX_LEN], size_t line_count, Program* vm_program)
{
    for (size_t i = 0; i < line_count; i++)
    {
        char* tokens[4];
        int token_count = 0;
        char* tok = strtok(lines[i], " ");
        while (tok && token_count < 4)
        {
            tokens[token_count++] = tok;
            tok = strtok(NULL, " ");
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
            if (!parse_register_comma_required(tokens[1], &instr.a))
            {
                printf("PARSTIME[MOV]Invalid value at line: %lu\n", i + 1);
                return 1;
            }

            if (!parse_int(tokens[2], &instr.b))
            {
                printf("PARSTIME[MOV]Invalid immediate at line: %lu\n", i + 1);
                return 1;
            }
            break;

        case OP_ADD:
        case OP_SUB:
            // ADD Rd, Rs1, Rs2
            if (!parse_register_comma_required(tokens[1], &instr.a))
            {
                printf("PARSTIME[ADD/SUB]Invalid value at line: %lu\n", i + 1);
                return 1;
            }
            if (!parse_register_comma_required(tokens[2], &instr.b))
            {
                printf("PARSTIME[ADD/SUB]Invalid value at line: %lu\n", i + 1);
                return 1;
            }
            if (!parse_register(tokens[3], &instr.c))
            {
                printf("PARSTIME[ADD/SUB]Invalid value at line: %lu\n", i + 1);
                return 1;
            }
            break;

        case OP_PRINT:
            // PRINT Rn
            if (!parse_register(tokens[1], &instr.a))
            {
                printf("PARSTIME[PRINT]Invalid registers at line: %lu\n", i + 1);
                return 1;
            }
            break;

        case OP_HALT:
            break;

        case OP_UNKNOWN:
            printf("Error parsing: Unknown opcode at %lu\n", line_count + 1);
            break;

        default:
            printf("Error parsing: Unspecified error at %lu\n", line_count + 1);
            break;
        }

        vm_program->program[vm_program->program_size] = instr;
        (vm_program->program_size)++;
    }

    return 0;
}
