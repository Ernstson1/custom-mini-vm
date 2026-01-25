#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_LABELS 100
#define MAX_LABEL_LEN 100

typedef struct
{
    size_t count;
    char keys[MAX_LABELS][MAX_LABEL_LEN];
    size_t values[MAX_LABELS];
} SymbolTable;

// Initialize/reset the symbol table
void symtab_init(SymbolTable* t);

// Insert a new symbol. Returns false if the table is full or if the key already exists.
bool symtab_add(SymbolTable* t, const char* key, size_t value);

// Lookup a symbol. Returns true if found and writes the value to out_value.
bool symtab_find(const SymbolTable* t, const char* key, size_t* out_value);

#endif // SYMBOL_TABLE_H
