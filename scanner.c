#include <stdio.h>
#include "scanner.h"
#include "token.h"
#include "token_list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
            while (self->source->str[look_ahead] != '\0') {
                if (self->source->str[look_ahead] == '#') {
                    look_ahead++;
                } else {
                    break;
                }
            }
            current = look_ahead - 1;
            // Make a substring with the text
            NCString *sub_string = nc_string_substring_in_range(self->source, i, current);
            // Headers start at 2 so add that offset, then the length to figure out which H level
            TokenType header_level = 2 + (current - i);
            // Scan the token in
            scan_token(self, sub_string, header_level);
            i = current;
        } else if (current_char == '*') {
            // Emphasis 
            TokenType tt = E1;
            char *e_str = malloc(sizeof(char) * 3);
            strcpy(e_str, "*");
            size_t e_len = sizeof(char) * 1;

            int look_ahead = i + 1;
            if (self->source->str[look_ahead] != '\0') {
                if (self->source->str[look_ahead] == '*') {
                    tt = E2;
                    strcpy(e_str, "**");
                    e_len = sizeof(char) * 2;
                    i++;
                }
            }

            NCString *e_nc_string = nc_string_init(e_str, e_len);
            scan_token(self, e_nc_string, tt);

        } else if (current_char == '\n') {
            // New line
            NCString *sub_string = nc_string_init("\\n", sizeof(char));
            // Scan the token in
            scan_token(self, sub_string, NL);
        } else if (current_char == '\0') {
            // End of file
            NCString *sub_string = nc_string_init("\\0", sizeof(char));
            scan_token(self, sub_string, EF);
            break;
        } else {
            // Text content
            int look_ahead = i + 1;
            while (self->source->str[look_ahead] != '\0') {
                if (!is_special_char(self->source->str[look_ahead])) {
                    look_ahead++;
                } else {
                    break;
                }
            }

            current = look_ahead - 1;
            // Make a substring with the text
            NCString *sub_string = nc_string_substring_in_range(self->source, i, current);
            // Scan the token in
            scan_token(self, sub_string, P);
            i = current;
        }
        i++;
    }
}

void scanner_dump(const Scanner* self) {
    nc_string_dump(self->source);
    printf("\n");
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


