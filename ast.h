#ifndef ast_h
#define ast_h

#include <stdio.h>
#include "../lib/nc_string.h"
#include "token.h"

typedef struct AST AST;

// ASTList
typedef struct ASTList {
    int size;   // How many elements the array can hold
    int count;    // How many are actually in the array.
    struct AST* nodes;
} ASTList;

// Code will switch between enum members to determine which part of the union to use. 
struct AST {
    enum {
        DOCUMENT,
        BLOCK,
        HEADER,
        PARAGRAPH,
        FORMATTED_TEXT,
        TEXT,
        EMPHASIS,
        EMPTY_LINE,
        TAG_EOF,
        HEADER_LEVEL
    } tag;

    union {
        // Document := [ Block ]
        struct NodeDocument {
            ASTList *blocks; // Represents an array of block elements, headers or paragraphs
        } NodeDocument;

        // Block := Header | EmptyLine | Paragraph
        struct NodeBlock {
            AST *block;
        } NodeBlock;

        // Header := HeaderLevel FormattedText EmptyLine
        struct NodeHeader {
            AST *headerLevel; 
            AST *formatted_text;
            AST *next_block;
        } NodeHeader;

        // HeaderLevel := '#' | '##' | '###' | '####' | '#####' | '######'
        struct NodeHeaderlevel {
            TokenType hl; 
        } NodeHeaderLevel;

        // Paragraph := [ Formatted Text ]
        struct NodeParagraph {
            AST *formatted_text;
            AST *next_block;
        } NodeParagraph;

        // FormattedText := Emphasis | Text
        struct NodeFormattedText {
            AST *text; 
            AST *emphasis; 
        } NodeFormattedText;

        // Text := Text
        NCString *text;

        // Emphasis := Text | FormattedText
        struct NodeEmphasis {
            AST *text; 
            AST *formatted_text;
        } NodeEmphasis;

        // EmptyLine := '\n'
        struct NodeEmptyLine {
            TokenType nl;
            AST *next_block;
        } NodeEmptyLine;

        // EndOfFile := '\0'
        TokenType eol;

    } data;
};
// AST API
struct AST* ast_init(); 
void ast_dump(const struct AST* self);
void ast_free(struct AST* self);

// ASTList API
struct ASTList* ast_list_init();
void ast_list_append(struct ASTList* self, struct AST a);
void ast_list_dump(const struct ASTList* self);
void ast_list_free(ASTList* self);

#endif /*ast_h*/
