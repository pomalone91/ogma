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
        case EMPTY_LINE:
            printf("EMPTY LINE\n");
            ast_dump(self->data.NodeEmptyLine.next_block);
        case TAG_EOF:
            printf("END OF FILE\n");
        case HEADER_LEVEL:
            printf("%d\n", self->data.NodeHeaderLevel.hl);
        // Non-leaf nodes require recursive descent
        case DOCUMENT:
            ast_list_dump(self->data.NodeDocument.blocks);
        case BLOCK:
            printf("Printing Block!\n");
        case HEADER:
            ast_dump(self->data.NodeHeader.headerLevel);
            ast_dump(self->data.NodeHeader.formatted_text);
            ast_dump(self->data.NodeHeader.next_block);
        case PARAGRAPH:
            ast_dump(self->data.NodeParagraph.formatted_text);
            ast_dump(self->data.NodeParagraph.next_block);
        case FORMATTED_TEXT:
            ast_dump(self->data.NodeFormattedText.text);
            ast_dump(self->data.NodeFormattedText.emphasis);
        case EMPHASIS:
            ast_dump(self->data.NodeEmphasis.text);
            ast_dump(self->data.NodeEmphasis.formatted_text);
    }
}

void ast_free(struct AST* self) {

    switch (self->tag) {
        // Leaf nodes
        case TEXT: 
            nc_string_free(self->data.text);
            free(self);
        case EMPTY_LINE:
            ast_free(self->data.NodeEmptyLine.next_block);
            free(self);
        case TAG_EOF:
            printf("END OF FILE\n");
        case HEADER_LEVEL:
            free(self);
        // Non-leaf nodes require recursive descent
        case DOCUMENT:
            ast_list_free(self->data.NodeDocument.blocks);
            free(self);
        case BLOCK:
            printf("Printing Block!\n");
        case HEADER:
            ast_free(self->data.NodeHeader.headerLevel);
            ast_free(self->data.NodeHeader.formatted_text);
            ast_free(self->data.NodeHeader.next_block);
            free(self);
        case PARAGRAPH:
            ast_free(self->data.NodeParagraph.formatted_text);
            ast_free(self->data.NodeParagraph.next_block);
            free(self);
        case FORMATTED_TEXT:
            ast_free(self->data.NodeFormattedText.text);
            ast_free(self->data.NodeFormattedText.emphasis);
            free(self);
        case EMPHASIS:
            ast_free(self->data.NodeEmphasis.text);
            ast_free(self->data.NodeEmphasis.formatted_text);
            free(self);
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
