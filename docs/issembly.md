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

### Examples
```asm
ADD R1, R2, R3
MOV R0, 10
PRINT R3
HALT
```
Invalid: 
```asm
ADD R1 R2 R3      ; missing commas
SUB R2, R1 R4    ; missing comma
```
## 2. Registers

- The VM provides 8 general-purpose registers
- Valid registers are R0 through R7
- Registers outside this range are invalid and cause a parse-time error

## 3. Operands and Semantics
- Operand Types
- Register: `R<number>`
- Immediate: signed 32-bit integer

### Destination Rule
For all arithmetic instructions:  
The destination register is always the last operand.  
Example: 
```asm
ADD R1, R2, R3
```
Semantics:
```asm
R3 = R1 + R2
```

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
Loads and immediate value into a register
- imm must fit in a signed 32-bit integer
- The destination register is overwritten

### 4.2 ADD

```asm
ADD R1, R2, R3
```
Add R2 from R1 and store result in R3

### 4.2 SUB

```asm
SUB R1, R2, R3
```
subtract R2 from R1 and store result in R3


### 4.4 PRINT
```asm
PRINT Rn
```
Print the stored value in the register to standart output.
- Output is written to `stdout`
- Intended to print the result during debugging

### 4.5 HALT
```asm
HALT
```
Terminates the program execution
- Takes no operands
- Marks normal program completion


## 5. Error Handlig
- The assembler rejects programs containing:
- Unknown instructions
- Invalid or out-of-range registers
- Invalid immediate values
- Missing or extra operands
- Missing commas between operands
- Operands containing unexpected whitespace
- Programs that fail validation are not executed by the VM.
