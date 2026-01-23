# Issembly Language Specification

This document specifies the syntax and semantics of the assembly-like language called Issembly supported by the Custom Mini Virtual Machine.

The language is intentionally minimal, strict, and unambiguous.

## 1. General Syntax

- One instruction per line
- Instructions are case-sensitive and must be written in uppercase
- Operands are separated by commas
- Whitespace is allowed around commas
- Empty lines are allowed
- The assembler performs strict validation; invalid programs are rejected

### Valid Examples
```asm
ADD R1, R2, R3
MOV R0, 10
PRINT R3
HALT
```

### Invalid Examples
```asm
ADD R1 R2 R3      ; Invalid: missing commas between operands
SUB R2, R1 R4     ; Invalid: missing comma before R4
add R1, R2, R3    ; Invalid: lowercase instruction
ADD R8, R1, R2    ; Invalid: R8 does not exist (only R0-R7)
```
## 2. Registers

- The VM provides 8 general-purpose registers
- Valid registers are: `R0`, `R1`, `R2`, `R3`, `R4`, `R5`, `R6`, `R7`
- Any register reference outside this range (e.g., `R8`, `R-1`, `R10`) is rejected as a **parse-time error**
- The program will not execute if it contains invalid register references

## 3. Operands and Semantics

### Operand Types
- **Register**: `R<number>` where number is 0-7
- **Immediate**: A signed 32-bit integer literal (e.g., `42`, `-10`, `0`)

### Destination Rule
For all arithmetic instructions, the **destination register is always the last operand**.  

Example: 
```asm
ADD R1, R2, R3
```

**Semantics**: 
```
R3 = R1 + R2
```
The VM reads from R1 and R2, computes the sum, and writes the result to R3.

## 4. Instruction Set
### Instruction Overview
| Instruction | Syntax           | Description                  |
| ----------- | ---------------- | ---------------------------- |
| `MOV`       | `MOV Rn, imm`    | Load immediate into register |
| `ADD`       | `ADD R1, R2, R3` | Add two registers            |
| `SUB`       | `SUB R1, R2, R3` | Subtract registers           |
| `PRINT`     | `PRINT Rn`       | Print register value         |
| `HALT`      | `HALT`           | Stop execution               |

### 4.1 MOV

```asm
MOV Rn, imm
```
Loads an immediate value into a register.

- `imm` must fit in a signed 32-bit integer
- The destination register is overwritten
- **MOV does not modify any flags**

### 4.2 ADD

```asm
ADD R1, R2, R3
```
Adds R1 and R2, and stores the result in R3.

**Semantics**: `R3 = R1 + R2`

- **Updates the zero flag (Z)** if the result is zero

### 4.3 SUB

```asm
SUB R1, R2, R3
```
Subtracts R2 from R1, and stores the result in R3.

**Semantics**: `R3 = R1 - R2`

- **Updates the zero flag (Z)** if the result is zero


### 4.4 PRINT

```asm
PRINT Rn
```
Prints the value stored in register `Rn` to standard output.

- Output is written to `stdout`
- Intended for debugging and inspecting register values during execution
- **Does not modify any flags**

### 4.5 HALT

```asm
HALT
```
Terminates program execution.

- Takes no operands
- Marks **normal program completion**
- Sets the VM execution status to `HALTED`
- **Does not modify any flags**


## 5. Flags

The VM maintains a set of status flags that are updated by certain instructions.

### Zero Flag (Z)
- Set to `1` if the result of an operation is zero
- Set to `0` otherwise

**Instructions that update the Zero Flag**:
- `ADD`
- `SUB`

**Instructions that do NOT modify flags**:
- `MOV`
- `PRINT`
- `HALT`

## 6. Program Termination

Programs can terminate in two ways:

### Normal Termination (HALT)
- The program executes a `HALT` instruction
- VM status: `HALTED`
- This is the expected way for a program to finish

### End-of-File Termination (EOF)
- The program counter reaches the end of the instruction list without encountering `HALT`
- VM status: `EOF`
- This indicates the program ended without explicitly halting

Both termination modes stop execution. The distinction is useful for tooling, testing, and future bytecode formats.

## 7. Error Handling

Errors are categorized into two types:

### Parse-Time Errors
The parser rejects programs containing:
- Unknown instructions
- Invalid or out-of-range registers (e.g., `R8`, `R-1`)
- Invalid immediate values (e.g., non-numeric literals)
- Missing or extra operands
- Missing commas between operands
- Syntax errors (e.g., lowercase instructions)

**Programs that fail parsing are not executed by the VM.**

### Runtime Errors
The VM may encounter errors during execution:
- Integer overflow (currently not checked, future consideration)
- Invalid memory access (not applicable in current design)
- Stack underflow/overflow (for future CALL/RET instructions)

**Runtime errors stop VM execution immediately.**
