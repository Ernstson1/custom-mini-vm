#include "symbol_table.h"

#include <string.h>

static bool symtab_index_of(const SymbolTable* t, const char* key, size_t* out_index)
{
    if (!t || !key) {
        return false;
    }

    for (size_t i = 0; i < t->count; i++) {
        if (strcmp(t->keys[i], key) == 0) {
            if (out_index) {
                *out_index = i;
            }
            return true;
        }
    }

    return false;
}

void symtab_init(SymbolTable* t)
{
    if (!t) {
        return;
    }

    t->count = 0;
    // No need to clear keys/values for correctness; count controls validity.
}

bool symtab_add(SymbolTable* t, const char* key, size_t value)
{
    if (!t || !key) {
        return false;
    }

    // Enforce capacity
    if (t->count >= MAX_LABELS) {
        return false;
    }

    // Reject duplicates
    if (symtab_index_of(t, key, NULL)) {
        return false;
    }

    // Reject overly long keys (must fit incl. NUL)
    const size_t len = strnlen(key, MAX_LABEL_LEN);
    if (len == MAX_LABEL_LEN) {
        // No NUL found within MAX_LABEL_LEN => too long
        return false;
    }

    memcpy(t->keys[t->count], key, len + 1); // include NUL
    t->values[t->count] = value;
    t->count++;

    return true;
}

bool symtab_find(const SymbolTable* t, const char* key, size_t* out_value)
{
    size_t idx;
    if (!symtab_index_of(t, key, &idx)) {
        return false;
    }

    if (out_value) {
        *out_value = t->values[idx];
    }

    return true;
}
