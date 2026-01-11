# Custom Mini VM  
A custom minimal virtual machine written in C, designed to execute a small, stack-based instruction set. 
This project allows you to write simple programs in a text-based “assembly-like” language, which the VM parses, interprets, and executes.  

The VM features a program counter, a small number of registers, a stack for function calls,
and basic instructions such as arithmetic operations, jumps, and subroutine calls.
It is structured to separate parsing, instruction memory, and execution logic, 
providing a clear foundation for extending the system with additional instructions, 
memory handling, or even binary bytecode support in the future.

## Features

### Core Features
- Text-based program loader for assembly-like source files
- Program counter (PC)–driven execution model
- Small register set for temporary values
- Stack-based execution with support for subroutines
- Basic instruction set (arithmetic, control flow, output)
- Fetch–decode–execute execution loop
- Clear separation between parsing, program memory, and execution

### Planned / Future Features
- Labels and jump instructions (`JMP`, conditional jumps)
- CALL / RET instructions with call stack support
- Improved error handling and validation during parsing and execution
- Optional memory instructions (`LOADM`, `STOREM`)
- Binary bytecode format for faster execution
- Execution debugging (step mode, register/stack inspection)
- Performance optimizations and cleaner instruction encoding
