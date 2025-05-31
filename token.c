#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"
#include "../lib/nc_string.h"

#define BASE_STR_SIZE 10

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
    printf("%s\n", self->str->str);
    printf("%d\n", self->type);
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
