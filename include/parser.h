#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction.h"
#include "program.h"

#define MAX_LEN 50

OpCode get_opcode(const char* token);

int parse_lines(char lines[][MAX_LEN], size_t line_count, Program* vm_program);

#endif // PARSER_H
