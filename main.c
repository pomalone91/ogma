#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "token.h"
#include "../lib/nc_string.h"
#include "ast.h"

int main (void) {
    
    // # Hello World\nHey!
    AST *a = ast_init();
    a->tag = DOCUMENT;
    a->data.eol = EF;

    // Literals leave nodes
    // Strings to build the literals with later

    char *str1 = "Hey!";
    size_t len1 = sizeof(char) * 5;
    NCString *ncstr1 = nc_string_init(str1, len1);
    AST *leaf1 = ast_init();
    leaf1->tag = TEXT;
    leaf1->data.text = ncstr1;

    char *str2 = "Hello World!";
    size_t len2 = sizeof(char) * 12;
    NCString *ncstr2 = nc_string_init(str2, len2);
    AST *leaf2 = ast_init();
    leaf2->tag = TEXT;
    leaf2->data.text = ncstr2;

    AST *ftext1 = ast_init();
    ftext1->tag = FORMATTED_TEXT;
    ftext1->data.NodeFormattedText.text = leaf1;
    ftext1->data.NodeFormattedText.emphasis = NULL;

    AST *ftext2 = ast_init();
    ftext2->tag = FORMATTED_TEXT;
    ftext2->data.NodeFormattedText.text = leaf2;
    ftext2->data.NodeFormattedText.emphasis = NULL;

    AST *paragraph = ast_init();
    paragraph->tag = PARAGRAPH;
    paragraph->data.NodeParagraph.formatted_text = ftext2;
    paragraph->data.NodeParagraph.next_block = NULL;

    AST *header_level = ast_init();
    header_level->tag = HEADER_LEVEL;
    header_level->data.NodeHeaderLevel.hl = H1;

    AST *header = ast_init();
    header->tag = HEADER;
    header->data.NodeHeader.formatted_text = ftext1;
    header->data.NodeHeader.headerLevel = header_level;
    header->data.NodeHeader.next_block = paragraph;

    ASTList *ast_list = ast_list_init();
    ast_list_append(ast_list, *header);
    ast_list_append(ast_list, *paragraph);

    AST *doc = ast_init();
    doc->tag = DOCUMENT;
    doc->data.NodeDocument.blocks = ast_list;

    // Try a dump
    ast_dump(doc);

    // Free stuff yipeeeeeee. Oh no the other free. 
    // Freeing doc should recursively free everything else???
    ast_free(doc);

    printf("%d, %d\n", a->tag, a->data.eol);

    /**************************************************************/
    // char *str = "# This is some markdown\n## Hell yeah. More text!\n### Header 3 baby!!!!";
    // //char *str = "# This *is* some markdown\n## Body\nLorem ipsum dolor sit amet, *consectetur* adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim **veniam**, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n";
    // size_t len = strlen(str);
    // NCString *string = nc_string_init(str, len);
    // Scanner *s = scanner_init();
    // s->source = string;

    // scanner_scan(s);
    // scanner_dump(s);

    // scanner_free(s);

    /**************************************************************/
    //const char *str1 = "A string!";
    //size_t len1 = strlen(str1);
    //NCString *string1 = nc_string_init(str1, len1);

    //const char *str2 = "Another string!";
    //size_t len2 = strlen(str2);
    //NCString *string2 = nc_string_init(str2, len2);

    //const char *str3 = "A third string.";
    //size_t len3 = strlen(str3);
    //NCString *string3 = nc_string_init(str3, len3);

    //const char *str4 = "A fourth string";
    //size_t len4 = strlen(str4);
    //NCString *string4 = nc_string_init(str4, len4);

    //Token *t1 = token_init_with_components(string1, BOLD);
    //Token *t2 = token_init_with_components(string2, H1);
    //Token *t3 = token_init_with_components(string3, URL);
    //Token *t4 = token_init_with_components(string4, H3);

    //TokenList *tl = token_list_init();
    //token_list_append(tl, *t1);
    //token_list_append(tl, *t2);
    //token_list_append(tl, *t3);
    //token_list_append(tl, *t4);
    //
    //token_list_dump(tl);

    //token_free(t1);
    //token_free(t2);
    //token_free(t3);
    //token_free(t4);
    //token_list_free(tl);
    
}
