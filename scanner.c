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

void scan_token(Scanner* self, NCString* ncs, TokenType t) {
    Token *token = token_init_with_components(ncs, t);
    token_list_append(self->tokens, *token);

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
        current_char = self->source->str[i];
        if (current_char == '#') {
            int look_ahead = i + 1;
            while (self->source->str[look_ahead] == '#') {
                look_ahead++;
            }
            // Make a substring with the text
            NCString *sub_string = nc_string_substring_in_range(self->source, i, look_ahead);
            // Headers start at 2 so add that offset, then the length to figure out which H level
            TokenType header_level = 2 + (look_ahead - i);
            // Scan the token in
            scan_token(self, sub_string, header_level);
        } else if (current_char == '\n') {
            // New line
            // Make a substring with the text
            NCString *sub_string = nc_string_substring_in_range(self->source, i, i);
            // Scan the token in
            scan_token(self, sub_string, NL);
        } else {
            // Text content
            int look_ahead = i + 1;
            while (!is_special_char(self->source->str[look_ahead])) {
                look_ahead++;
            }
            // Make a substring with the text
            NCString *sub_string = nc_string_substring_in_range(self->source, i, look_ahead);
            // Scan the token in
            scan_token(self, sub_string, P);
            i = look_ahead;
        }
        i++;
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


