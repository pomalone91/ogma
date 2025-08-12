#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"
#include "../lib/nc_string.h"

#define BASE_STR_SIZE 10

// Token Implementation
Token* token_init() {
    Token *t = malloc(sizeof(Token));
    t->type = 9999;
    t->str = nc_string_init_empty(); 
    return t;
}

Token* token_init_with_components(NCString* str, TokenType type) {
    Token *t = malloc(sizeof(Token));
    t->type = type;
    t->str = nc_string_init(str->str, str->len);
    return t;
}

void token_dump(const struct Token* self) {
    printf("Type: %d; Length: %zu; Content: '%s'\n", self->type, self->str->len, self->str->str);
}

void token_free(Token* self) {
    if (self) {
        self->type = 0;
        nc_string_free(self->str);
        self->str = NULL;
        free(self);
        self = NULL;
    }
}

// token_list Implementation
// Initialize a new array
struct TokenList* token_list_init() {
    TokenList *tk = malloc(sizeof(TokenList));
    tk->size = 1;
    tk->count = 0;
    //el->token = malloc(sizeof(Token) * 2);
    tk->token = token_init();
    return tk;
}

// Append a new token to array
void token_list_append(struct TokenList* self, Token t) {
    // If count is same as size we need to double the allocated memory.
    if (self->count == self->size) {
        self->size *= 2;
        size_t allocation_size = sizeof(Token) * self->size;
        Token *temp_token = realloc(self->token, sizeof(Token) * allocation_size);
        if (temp_token) {
            self->token = temp_token;
        } else {
            printf("Error reallocating token_list\n");
            return;
        }
    }
    (self->token + self->count)->str =  nc_string_init(t.str->str, t.str->len);
    (self->token + self->count)->type = t.type;

    self->count++;
}

void token_list_dump(const struct TokenList* self) {
    for (int i = 0; i < self->count; i++)
    {
        token_dump(self->token + i);
    }
}

void token_list_free(struct TokenList* self) {
    if (self->token) {
        token_free(self->token);
        self->token = NULL;
    }
    self->count = 0;
    self->size = 0;
    free(self);
    self = NULL;
}
