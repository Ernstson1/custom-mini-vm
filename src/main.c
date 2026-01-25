#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "parser.h"
#include "program.h"
#include "vm.h"

#define MAX_LINE 1000
#define MAX_LEN 50

#define MAX_PROGRAM 100

int main(int argc, char* argv[])
{
    // Init Logger
    Logger log;
    log_init(&log, stderr, LOG_INFO);

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

    // ========= COMPILE TIME =========
    Parser parser;
    ParserResult pars_res = parse_lines(&parser, lines, line, &vm_program);

    switch (pars_res)
    {
    case PARSER_OK:
        break;
    case PARSER_ERR_INVALID_VALUE:
        log_msg(&log, LOG_ERROR, "Parser failed: invalid value at line %zu", parser.last_line);
        return 1;
    case PARSER_ERR_INVALID_REGISTER:
        log_msg(&log, LOG_ERROR, "Parser failed: invalid register at line %zu", parser.last_line);
        return 1;
    case PARSER_ERR_INVALID_IMMEDIATE:
        log_msg(&log, LOG_ERROR, "Parser failed: invalid immediate at line %zu", parser.last_line);
        return 1;
    case PARSER_ERR_UNKNOWN_OPCODE:
        log_msg(&log, LOG_ERROR, "Parser failed: invalid opcode at line %zu", parser.last_line);
        return 1;
    case PARSER_ERR_PROGRAM_TOO_LARGE:
        log_msg(&log, LOG_ERROR, "Parser failed: Program to large at line %zu", parser.last_line);
        return 1;
    case PARSER_ERR_UNKNOWN_ERROR:
        log_msg(&log, LOG_ERROR, "Parser failed: unknown error at line %zu", parser.last_line);
        return 1;
    }

    VM vm;
    vm_init(&vm);
    
    // ========= RUN TIME =========

    VmResult res = vm_run(&vm, &vm_program);

    switch (res)
    {
    case VM_HALTED:
        return 0;

    case VM_EOF:
        log_msg(&log, LOG_WARN, "VM finished without HALT");
        return 0;

    case VM_ERR_PC_OOB:
        log_msg(&log, LOG_ERROR, "VM failed: PC out of bounds at pc=%zu", vm.fetch_pc);
        return 1;

    case VM_ERR_INVALID_OPCODE:
        log_msg(&log, LOG_ERROR, "VM failed: invalid opcode at pc=%zu", vm.fetch_pc);
        return 1;

    case VM_ERR_INVALID_REGISTER:
        log_msg(&log, LOG_ERROR, "VM failed: invalid register index at pc=%zu", vm.fetch_pc);
        return 1;

    case VM_ERR_STACK_OVERFLOW:
        log_msg(&log, LOG_ERROR, "VM failed: stack overflow at pc=%zu", vm.fetch_pc);
        return 1;

    case VM_ERR_STACK_UNDERFLOW:
        log_msg(&log, LOG_ERROR, "VM failed: stack underflow at pc=%zu", vm.fetch_pc);
        return 1;

    default:
        log_msg(&log, LOG_ERROR, "VM failed: unknown result code (%d)", (int)res);
        return 1;
    }
}
