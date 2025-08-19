#ifndef md_doc_h
#define md_doc_h

#include <stdio.h>
#include "../lib/nc_string.h"
#include "token.h"
#include "ast.h"

// Scanner 
typedef struct MDDoc {
    NCString* source;
    TokenList* tokens;
    AST* tree;
} MDDoc;

struct MDDoc* md_doc_init();
void md_doc_dump(const MDDoc* self);
void md_doc_free(MDDoc* self);


#endif /*scanner*/
