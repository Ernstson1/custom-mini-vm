#include "parser.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction.h"
#include "program.h"
#include "symbol_table.h"

#define MAX_LEN 50
#define MAX_REG_COUNT 8

int parser_init(Parser* p, SymbolTable* t)
{
    if (!p)
        return 0;

    if (!t)
        return 0;

    p->t = *t;
    return 1;
}

OpCode get_opcode(const char* token)
{
    size_t len = token ? strlen(token) : 0;
    // Label syntax: name:
    if (len > 0 && token[len - 1] == ':')
        return LABEL;

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
    if (strcmp(token, "JMP") == 0)
        return OP_JMP;
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
        if (len < 2) // "R," är ogiltigt
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

ParserResult parse_lines(Parser* parser, char lines[][MAX_LEN], size_t line_count,
                         Program* vm_program)
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

        // Store 1-based source line number for user-facing errors
        parser->last_line = i + 1;

        // Labels are not instructions: they do not emit code and do not advance PC.
        if (instr.opcode == LABEL)
        {
            const size_t tlen = strlen(tokens[0]);
            // Must be at least "a:" (2 chars)
            if (tlen < 2 || tokens[0][tlen - 1] != ':')
                return PARSER_ERR_INVALID_VALUE;

            // Strip trailing ':'
            tokens[0][tlen - 1] = '\0';

            // Store current instruction index as label target
            const size_t pc = vm_program->program_size;
            if (!symtab_add(&parser->t, tokens[0], pc))
                return PARSER_ERR_INVALID_VALUE; // duplicate label / table full / key too long

            continue;
        }

        instr.a = instr.b = instr.c = 0;

        switch (instr.opcode)
        {
        case OP_MOV:
            // MOV Rn, value
            if (!parse_register_comma_required(tokens[1], &instr.a))
            {
                return PARSER_ERR_INVALID_REGISTER;
            }

            if (!parse_int(tokens[2], &instr.b))
            {
                return PARSER_ERR_INVALID_IMMEDIATE;
            }
            break;

        case OP_ADD:
        case OP_SUB:
            // ADD Rd, Rs1, Rs2
            if (!parse_register_comma_required(tokens[1], &instr.a))
            {
                return PARSER_ERR_INVALID_REGISTER;
            }
            if (!parse_register_comma_required(tokens[2], &instr.b))
            {
                return PARSER_ERR_INVALID_REGISTER;
            }
            if (!parse_register(tokens[3], &instr.c))
            {
                return PARSER_ERR_INVALID_REGISTER;
            }
            break;

        case OP_PRINT:
            // PRINT Rn
            if (!parse_register(tokens[1], &instr.a))
            {
                return PARSER_ERR_INVALID_REGISTER;
            }
            break;
        case OP_HALT:
            break;

        case OP_UNKNOWN:
            return PARSER_ERR_UNKNOWN_OPCODE;

        default:
            return PARSER_ERR_UNKNOWN_ERROR;
        }

        if (vm_program->program_size >= MAX_PROGRAM)
            return PARSER_ERR_PROGRAM_TOO_LARGE;

        vm_program->program[vm_program->program_size] = instr;
        (vm_program->program_size)++;
    }

    return PARSER_OK;
}
