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

// Helper stuff
bool is_special_char(char c) {
    bool special;
    switch (c)
    {
    case '#':
        special = true;
        break;
    case '\n':
        special = true;
        break;
    case '*':
        special = true;
        break;
    case '[':
        special = true;
        break;
    case ']':
        special = true;
        break;
    case '(':
        special = true;
        break;
    case ')':
        special = true;
        break;
    case '_':
        special = true;
    default:
        special = false;
        break;
    }
    return special;
}

// Helper function for scannering tokens
Token* scan_token(NCString* ncs, TokenType t) {
    Token *token = token_init_with_components(ncs, t);
    return token;
}

// Scan a list of tokens from a string
struct TokenList* token_list_scan(NCString *source) {
    TokenList *tl = token_list_init();
    int i = 0;
    int start = 0; // The start of the lexeme or whatever it's called
    int current = 0; // The current position if we have to scan ahead
    char current_char = source->str[i];

    while (current_char != '\0') {
        current_char = source->str[i];
        if (current_char == '#') {
            int look_ahead = i + 1;
            while (source->str[look_ahead] != '\0') {
                if (source->str[look_ahead] == '#') {
                    look_ahead++;
                } else {
                    break;
                }
            }
            current = look_ahead - 1;
            // Make a substring with the text
            NCString *sub_string = nc_string_substring_in_range(source, i, current);
            // Headers start at 2 so add that offset, then the length to figure out which H level
            TokenType header_level = 2 + (current - i);
            // Scan the token in
            token_list_append(tl, *scan_token(sub_string, header_level));
            i = current;

        } else if (current_char == '*') {
            // Emphasis 
            TokenType tt = E1;
            char *e_str = malloc(sizeof(char) * 3);
            strcpy(e_str, "*");
            size_t e_len = sizeof(char) * 1;

            int look_ahead = i + 1;
            if (source->str[look_ahead] != '\0') {
                if (source->str[look_ahead] == '*') {
                    tt = E2;
                    strcpy(e_str, "**");
                    e_len = sizeof(char) * 2;
                    i++;
                }
            }

            NCString *e_nc_string = nc_string_init(e_str, e_len);
            token_list_append(tl, *scan_token(e_nc_string, tt));

        } else if (current_char == '\n') {
            // New line
            NCString *sub_string = nc_string_init("\\n", sizeof(char));
            // Scan the token in
            token_list_append(tl, *scan_token(sub_string, NL));
        } else if (current_char == '\0') {
            // End of file
            NCString *sub_string = nc_string_init("\\0", sizeof(char));
            token_list_append(tl, *scan_token(sub_string, EF));
            break;
        } else {
            // Text content
            int look_ahead = i + 1;
            while (source->str[look_ahead] != '\0') {
                if (!is_special_char(source->str[look_ahead])) {
                    look_ahead++;
                } else {
                    break;
                }
            }

            current = look_ahead - 1;
            // Make a substring with the text
            NCString *sub_string = nc_string_substring_in_range(source, i, current);
            // Scan the token in
            token_list_append(tl, *scan_token(sub_string, P));
            i = current;
        }
        i++;
    }

    return tl;
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
