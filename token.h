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
    BOLD,
    ITALIC,
    URL,
    IMG,
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

#endif /*token_h*/
