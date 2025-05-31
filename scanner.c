#include <stdio.h>
#include "scanner.h"
#include "token.h"
#include "token_list.h"
#include <stdlib.h>
#include <stdbool.h>

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

void scan_token(Scanner* self, char c) {
    if (c == '#') {
        NCString *s = nc_string_init("#", sizeof(char));
        Token *t = token_init_with_components(s, H1);
        token_list_append(self->tokens, *t);
        token_free(t);
        nc_string_free(s);

    } else if (c == '\n') {
        // New line
        NCString *s = nc_string_init("\n", sizeof(char));
        Token *t = token_init_with_components(s, NL);
        token_list_append(self->tokens, *t);
        token_free(t);
        nc_string_free(s);
    }
}

struct Scanner* scanner_init() {
    Scanner *s = malloc(sizeof(Scanner));
    s->tokens = token_list_init();
    s->source = nc_string_init_empty();

    return s;
}

void scanner_scan(Scanner* self) {
    int i = 0;
    int start = 0; // The start of the lexeme or whatever it's called
    int current = 0; // The current position if we have to scan ahead
    char current_char = self->source->str[i];

    while (current_char != '\0') {
        scan_token(self, current_char);
        i++;
        current_char = self->source->str[i];
    }
}

void scanner_dump(const Scanner* self) {
    nc_string_dump(self->source);
    token_list_dump(self->tokens);
}

void scanner_free(Scanner* self) {
    if (self) {
        nc_string_free(self->source);
        self->source = NULL;
        token_list_free(self->tokens);
        self->tokens = NULL;
        free(self);
        self = NULL;
    }
}


