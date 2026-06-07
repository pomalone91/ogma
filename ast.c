#include "ast.h"
#include "token.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// AST implementation
struct AST* ast_init() {
    AST *a = malloc(sizeof(AST));
    if (a == NULL) { return NULL; }
    return a;
}

void ast_dump(const struct AST* self) {
    // If were at a leaf node we should print it. Otherwise we need to traverse stuff. 
    switch (self->tag) {
        // Leaf nodes
        case TEXT: 

            printf("TEXT: ");
            printf("%s\n", self->data.text->str);
            return;
        case END_OF_LINE:
            printf("EMPTY LINE\n");
            return;
        case TAG_EOF:
            printf("END OF FILE\n");
            return;
        case HEADER_LEVEL:
            printf("HEADER LEVEL: ");
            printf("%d\n", self->data.HeaderLevel.hl);
            return;
        // Non-leaf nodes require recursive descent
        case DOCUMENT:
            if (self->data.Document.blocks)
                ast_list_dump(self->data.Document.blocks);
            return;
        case BLOCK:
            printf("Printing Block!\n");
            return;
        case HEADER:
            printf("HEADER: ");
            if (self->data.Header.header_level) {
                ast_dump(self->data.Header.header_level);
            }
            if (self->data.Header.formatted_text) {
                ast_dump(self->data.Header.formatted_text);
            }
            return;
        case PARAGRAPH:
            printf("PARAGRAPH: ");
            if (self->data.Paragraph.formatted_text) {
                ast_dump(self->data.Paragraph.formatted_text);
            }
            return;
        case FORMATTED_TEXT:
            printf("FORMATTED TEXT: ");
            if (self->data.FormattedText.text) {
                ast_dump(self->data.FormattedText.text);
            }
            if (self->data.FormattedText.emphasis) {
                ast_dump(self->data.FormattedText.emphasis);
            }
            return;
        case EMPHASIS:
            printf("EMPHASIS : ");
            if (self->data.Emphasis.text) {
                ast_dump(self->data.Emphasis.text);
            }
            if (self->data.Emphasis.formatted_text) {
                ast_dump(self->data.Emphasis.formatted_text);
            }
            return;
    }
}

void ast_free(struct AST* self) {
    switch (self->tag) {
        // Leaf nodes
        case TEXT: 
            printf("Freeing TEXT node\n");
            if (self) {
                if (self->data.text) {
                    nc_string_free(self->data.text);
                    self->data.text = NULL;
                }
                free(self);
                self = NULL;
            }
            return;
        case END_OF_LINE:
            if (self) {
                free(self);
                self = NULL;
            }
            return;
        case TAG_EOF:
            printf("END OF FILE\n");
            return;
        case HEADER_LEVEL:
            printf("Freeing HEADER_LEVEL node\n");
            if (self) {
                free(self);
                self = NULL;
            }
            return;
        // Non-leaf nodes require recursive descent
        case DOCUMENT:
            if (self) {
                if (self->data.Document.blocks) {
                    ast_list_free(self->data.Document.blocks);
                    self->data.Document.blocks = NULL;
                }
                //free(self);
                //self = NULL;
            }
            return;
        case BLOCK:
            printf("Printing Block!\n");
            return;
        case HEADER:
            printf("Freeing HEADER node\n");
            if (self) {
                if (self->data.Header.header_level) {
                    ast_free(self->data.Header.header_level);
                    self->data.Header.header_level = NULL;
                }
                if (self->data.Header.formatted_text) {
                    ast_free(self->data.Header.formatted_text);
                    self->data.Header.formatted_text = NULL;
                }
                free(self);
                self = NULL;
            }
            return;
        case PARAGRAPH:
            printf("Freeing PARAGRAPH node\n");
            if (self) {
                if (self->data.Paragraph.formatted_text) {
                    ast_free(self->data.Paragraph.formatted_text);
                    self->data.Paragraph.formatted_text = NULL;
                }
                if (self) {
                    free(self);
                    self = NULL;
                }
            }
            return;
        case FORMATTED_TEXT:
            printf("Freeing FORMATTED_TEXT node\n");
            if (self) {
                if (self->data.FormattedText.text) {
                    ast_free(self->data.FormattedText.text);
                    self->data.FormattedText.text = NULL;
                }
                if (self->data.FormattedText.emphasis) {
                    ast_free(self->data.FormattedText.emphasis);
                    self->data.FormattedText.emphasis = NULL;
                }
                free(self);
                self = NULL;
            }
            return;
        case EMPHASIS:
            if (self) {
                if (self->data.Emphasis.text) {
                    ast_free(self->data.Emphasis.text);
                    self->data.Emphasis.text = NULL;
                }
                if (self->data.Emphasis.formatted_text) {
                    ast_free(self->data.Emphasis.formatted_text);
                    self->data.Emphasis.formatted_text = NULL;
                }
                free(self);
                self = NULL;
            }
            return;
    }
}

// ASTList implementation
struct ASTList* ast_list_init() {
    ASTList *a = malloc(sizeof(ASTList));
    if (a == NULL) { return NULL; }

    a->count = 0;
    a->size = 1;
    a->nodes = ast_init();

    return a;
}

void ast_list_append(struct ASTList* self, struct AST a) {
    // If count is same as size we need to double the allocated memory.
    if (self->count == self->size) {
        self->size *= 2;
        size_t allocation_size = sizeof(AST) * self->size;
        AST *temp_ast = realloc(self->nodes, sizeof(AST) * allocation_size);
        if (temp_ast) {
            self->nodes = temp_ast;
        } else {
            printf("Error reallocating temp_ast\n");
            return;
        }
    }

    (self->nodes + self->count)->tag = a.tag;
    (self->nodes + self->count)->data = a.data;

    self->count++;
}

void ast_list_dump(const struct ASTList* self) {
    int *depth = malloc(sizeof(int));  // To track how deep in the tree we are to print tabs correctly
    *depth = -1;
    printf("ast_list count: %i\n", self->count);
    printf("ast_list size: %i\n", self->size);
    for (int i = 0; i < self->count; i++)
    {
        ast_dump(self->nodes + i);
    }
}

void ast_list_free(ASTList* self) {
    if (self->nodes) {
        ast_free(self->nodes);
        self->nodes = NULL;
    }
    self->size = 0;
    self->count = 0;
    free(self);
    self = NULL;
}

// Forward declarations if we need them
AST* parse_formatted_text(TokenList *tl, int *position, bool *emphasis_flag);

AST* parse_end_of_line(TokenList *tl, int *position, bool *emphasis_flag) {
    AST *eol = ast_init();
    eol->tag = END_OF_LINE;
    *position += 1;
    return eol;
}

AST* parse_text(TokenList *tl, int *position, bool *emphasis_flag) {
    AST *text = ast_init();
    text->tag = TEXT;
    text->data.text = nc_string_init((tl->token + *position)->str->str); // Owns its own copy of ncstring
    return text;
}

AST* parse_emphasis(TokenList *tl, int *position, bool *emphasis_flag) {
    AST *emph = ast_init();
    emph->tag = EMPHASIS;
    *position += 1;
    if ((tl->token + *position)->type == EMPHASIS) {
        emph->data.Emphasis.text = NULL;
    }

    // If we just entered an emphasis, parse it
    if (*emphasis_flag == false) {
        emph->data.Emphasis.text = parse_text(tl, position, emphasis_flag);
        *emphasis_flag = true;
    }

    // Check the next token...
    *position += 1;
    if ((tl->token + *position)->type == E1) {
        // The emphasis is over.
        *position += 1;
        *emphasis_flag = false;
    }
    if ((tl->token + *position)->type == NL) {
        emph->data.Emphasis.EndOfLine = parse_end_of_line(tl, position, emphasis_flag);
        emph->data.Emphasis.formatted_text = NULL;
    } else if ((tl->token + *position)->type == P) {
        emph->data.Emphasis.formatted_text = parse_formatted_text(tl, position, emphasis_flag);
        emph->data.Emphasis.EndOfLine = NULL;
    }
    return emph;
}

AST* parse_formatted_text(TokenList *tl, int *position, bool *emphasis_flag) {
    AST *ft = ast_init();
    ft->tag = FORMATTED_TEXT;
    // If the entry token is text just parse this one as text.
    if ((tl->token + *position)->type == P) {
        ft->data.FormattedText.text = parse_text(tl, position, emphasis_flag);
        *position += 1;
        // If there is an emphasis within the text
        if ((tl->token + *position)->type == E1) {
            ft->data.FormattedText.emphasis = parse_emphasis(tl, position, emphasis_flag);
        }
    } else if ((tl->token + *position)->type == E1){
        ft->data.FormattedText.emphasis = parse_emphasis(tl, position, emphasis_flag);
    } else {
        ft->data.FormattedText.text = NULL;
    }
    return ft;
}

AST* parse_paragraph(TokenList *tl, int *position, bool *emphasis_flag) {
    AST *paragraph = ast_init();
    paragraph->tag = PARAGRAPH;
    paragraph->data.Paragraph.formatted_text = parse_formatted_text(tl, position, emphasis_flag);

    return paragraph;
}

AST* parse_header(TokenList *tl, int *position, bool *emphasis_flag) {
    AST *header_level = ast_init();
    header_level->tag = HEADER_LEVEL;
    header_level->data.HeaderLevel.hl = tl->token->type;

    AST *h = ast_init();
    h->tag = HEADER;
    h->data.Header.header_level = header_level;
    // If there is another token in the list, check if it works as formatted text.
    *position += 1;
    if ((tl->token + *position)->type == NL) {
        h->data.Header.end_of_line = parse_end_of_line(tl, position, emphasis_flag);
        h->data.Header.formatted_text = NULL;
    }
    if ((tl->token + *position)->type == P) {
        h->data.Header.formatted_text = parse_formatted_text(tl, position, emphasis_flag);
        h->data.Header.end_of_line = NULL;
    } else {
        h->data.Header.formatted_text = NULL;
        h->data.Header.end_of_line = NULL;
    }

    return h;
}

AST* parse_node(TokenList *tl, int *position) {
    AST *ast = ast_init();

    return ast;
}
/*Recursively parsing the AST*/
ASTList* ast_parse(TokenList* tl) {
    int* position = malloc(sizeof(int));  // This is a pointer so that all the recursive functions will increment it to track where we are in the token list.
    *position = 0;
    bool *emphasis_flag = malloc(sizeof(bool));  // true if we are currently inside an emphasis and haven't found the end yet. 
    *emphasis_flag = false;
    ASTList *astl = ast_list_init();

    // Start at the top and parse recursively until we can't anymore. Return the position from each recursive function so that we know where we're at when we get back and can do the next block... I guess. Or maybe I should just parse the blocks as blocks first?
    while (*position < tl->count - 1) {
        printf("position: %i\n", *position);
        token_dump(tl->token + *position);
        switch ((tl->token + *position)->type) {
            case ERROR:
                printf("TODO\n");
                break;
            case P:
                printf("Parsing paragraph\n");
                ast_list_append(astl, *parse_paragraph(tl, position, emphasis_flag));
                break;
            case H1:
                printf("Parsing header\n");
                ast_list_append(astl, *parse_header(tl, position, emphasis_flag));
                break;
            case H2:
                printf("Parsing header\n");
                break;
            case H3:
                printf("Parsing header\n");
                break;
            case H4:
                printf("Parsing header\n");
                break;
            case H5:
                printf("Parsing header\n");
                break;
            case H6:
                printf("Parsing header\n");
                break;
            case NL:
                printf("TODO\n");
                break;
            case E1:
                printf("TODO\n");
                break;
            case E2:
                printf("Parsing Emphasis\n");
                ast_list_append(astl, *parse_emphasis(tl, position, emphasis_flag));
                break;
            case URL:
                printf("TODO\n");
                break;
            case IMG:
                printf("TODO\n");
                break;
            case EF:
                printf("TODO\n");
                break;
            default:
                printf("I haven't finished this yet\n");
        }
        *position += 1;
    }

    free(position);
    position = NULL;

    return astl;
}

