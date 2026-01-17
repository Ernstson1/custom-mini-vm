#ifndef PROGRAM_H
#define PROGRAM_H

#include "instruction.h"
#include <stddef.h>

#define MAX_PROGRAM 100

typedef struct {
    Instruction program[MAX_PROGRAM];
    size_t program_size;
} Program;

#endif // PROGRAM_H
