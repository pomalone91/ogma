#ifndef token_list_h
#define token_list_h

#include <stdio.h>
#include "token.h"

// TokenArray
typedef struct TokenList {
    int size;   // How many elements the array can hold
    int count;    // How many are actually in the array.
    struct Token* token;
} TokenList;

struct TokenList* token_list_init();
void token_list_append(struct TokenList* self, struct Token t);
void token_list_dump(const struct TokenList* self);
void token_list_free(TokenList* self);


#endif /*token_list*/
