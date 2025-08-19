#ifndef token_h
#define token_h

#include <stdio.h>
#include "../lib/nc_string.h"

// Token Types
typedef enum TokenType {
    ERROR,
    P,
    H1,
    H2,
    H3,
    H4,
    H5,
    H6,
    NL,
    E1,
    E2,
    URL,
    IMG,
    EF
} TokenType;

// Token
typedef struct Token
{
    TokenType type;
    NCString *str;
} Token;

struct Token* token_init(); 
struct Token* token_init_with_components(NCString* str, TokenType type);
void token_dump(const struct Token* self);
void token_free(struct Token* self);

// TokenArray
typedef struct TokenList {
    int size;   // How many elements the array can hold
    int count;    // How many are actually in the array.
    struct Token* token;
} TokenList;

struct TokenList* token_list_init();
struct TokenList* token_list_scan(NCString* src);
void token_list_append(struct TokenList* self, struct Token t);
void token_list_dump(const struct TokenList* self);
void token_list_free(TokenList* self);

#endif /*token_h*/
