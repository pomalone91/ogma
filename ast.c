#include "ast.h"
#include <stdlib.h>

// AST implementation
struct AST* ast_init() {
    AST *a = malloc(sizeof(AST));
    if (a) return a;
    else return NULL;
}

void ast_dump(const struct AST* self) {
    // If were at a leaf node we should print it. Otherwise we need to traverse stuff. 
    switch (self->tag) {
        // Leaf nodes
        case TEXT: 
            printf("%s\n", self->data.text->str);
            return;
        case EMPTY_LINE:
            printf("EMPTY LINE\n");
            if (self->data.EmptyLine.next_block)
                ast_dump(self->data.EmptyLine.next_block);
            return;
        case TAG_EOF:
            printf("END OF FILE\n");
            return;
        case HEADER_LEVEL:
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
            if (self->data.Header.headerLevel)
                ast_dump(self->data.Header.headerLevel);
            if (self->data.Header.formatted_text)
                ast_dump(self->data.Header.formatted_text);
            return;
        case PARAGRAPH:
            if (self->data.Paragraph.formatted_text)
                ast_dump(self->data.Paragraph.formatted_text);
            return;
        case FORMATTED_TEXT:
            if (self->data.FormattedText.text)
                ast_dump(self->data.FormattedText.text);
            if (self->data.FormattedText.emphasis)
                ast_dump(self->data.FormattedText.emphasis);
            return;
        case EMPHASIS:
            if (self->data.Emphasis.text)
                ast_dump(self->data.Emphasis.text);
            if (self->data.Emphasis.formatted_text)
                ast_dump(self->data.Emphasis.formatted_text);
            return;
    }
}

void ast_free(struct AST* self) {

    switch (self->tag) {
        // Leaf nodes
        case TEXT: 
            nc_string_free(self->data.text);
            free(self);
            return;
        case EMPTY_LINE:
            ast_free(self->data.EmptyLine.next_block);
            free(self);
            return;
        case TAG_EOF:
            printf("END OF FILE\n");
            return;
        case HEADER_LEVEL:
            free(self);
            return;
        // Non-leaf nodes require recursive descent
        case DOCUMENT:
            ast_list_free(self->data.Document.blocks);
            free(self);
            return;
        case BLOCK:
            printf("Printing Block!\n");
            return;
        case HEADER:
            ast_free(self->data.Header.headerLevel);
            ast_free(self->data.Header.formatted_text);
            free(self);
            return;
        case PARAGRAPH:
            ast_free(self->data.Paragraph.formatted_text);
            free(self);
            return;
        case FORMATTED_TEXT:
            ast_free(self->data.FormattedText.text);
            ast_free(self->data.FormattedText.emphasis);
            free(self);
            return;
        case EMPHASIS:
            ast_free(self->data.Emphasis.text);
            ast_free(self->data.Emphasis.formatted_text);
            free(self);
            return;
    }
}

// ASTList implementation
struct ASTList* ast_list_init() {
    ASTList *a = malloc(sizeof(ASTList));
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
    for (int i = 0; i < self->count; i++)
    {
        ast_dump(self->nodes + i);
    }
}

void ast_list_free(ASTList* self) {
    if (self->nodes) {
        for (int i = 0; i < self->count; i++) {
            ast_free(self->nodes + i);
        }
    }
    self->size = 0;
    self->count = 0;
    free(self);
    self = NULL;
}
