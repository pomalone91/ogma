#include "ast.h"
#include <stdlib.h>

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
            ast_dump(self->data.NodeDocument.blocks);
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
            ast_dump(self->data.NodeDocument.blocks);
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
