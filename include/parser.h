#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction.h"
#include "program.h"
#include "symbol_table.h"

#define MAX_LEN 50

typedef enum {
    PARSER_OK,
    PARSER_ERR_INVALID_VALUE,
    PARSER_ERR_INVALID_REGISTER,
    PARSER_ERR_INVALID_IMMEDIATE,
    PARSER_ERR_UNKNOWN_OPCODE,
    PARSER_ERR_PROGRAM_TOO_LARGE,
    PARSER_ERR_UNKNOWN_ERROR
} ParserResult;

typedef struct{
    SymbolTable t;
    int last_line;

} Parser;

int parser_init(Parser *p, SymbolTable *t);
OpCode get_opcode(const char* token);

ParserResult parse_lines(Parser* parser, char lines[][MAX_LEN], size_t line_count, Program* vm_program);

#endif // PARSER_H
