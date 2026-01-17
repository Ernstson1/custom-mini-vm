# Custom Mini VM

A small, system-level virtual machine written in C.

The VM executes programs written in a custom assembly-like language called **Issembly**.
It is designed as a learning and experimentation platform for virtual machines,
instruction sets, and low-level execution models.

The project emphasizes:
- clear separation between parsing, program representation, and execution
- explicit VM state (registers, stack, program counter)
- strict validation and deterministic behavior

---

## Issembly

**Issembly** is a minimal, strict, assembly-like language designed specifically for this VM.
It is intentionally simple and unambiguous to make parsing and execution predictable.

Only a small instruction set is currently supported, focusing on arithmetic,
basic I/O, and program termination.

For the full language specification, syntax rules, and instruction semantics, see:

➡ **[Issembly Language Specification](docs/issembly.md)**

---

## Example

```asm
MOV R1, 10
MOV R2, 32
ADD R1, R2, R3
PRINT R3
HALT
```
output: 
```
42
```
## Features
### Current

- Text-based loader for Issembly source files
- Program counter–driven execution model
- Fixed-size register file
- Stack support for subroutines (work in progress)
- Fetch–decode–execute execution loop
- Strict parse-time and runtime validation
- Clean separation between:
    - parser
    - program representation
    - VM execution core

### Planned
- Labels and control flow instructions (JMP, JZ, JNZ)
- Subroutine support (CALL, RET)
- Improved runtime error reporting
- Optional memory instructions
- Binary bytecode format
- Debug / trace mode
- Cleaner instruction encoding and optimizations  

## Project Goals
This project is primarily educational and exploratory.
It aims to provide a technically correct foundation for:

- understanding VM internals
- instruction set design
- low-level execution models in C

It is not intended to be compatible with any real assembly language.

