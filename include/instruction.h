typedef enum {
    OP_MOV,
    OP_ADD,
    OP_SUB,
    OP_PRINT,
    OP_HALT,
    OP_UNKNOWN
} OpCode;

typedef struct {
    OpCode opcode;
    int a; // t.ex. register eller adress
    int b; // t.ex. register eller immediate
    int c; // t.ex. register
} Instruction;
