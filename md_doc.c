#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "md_doc.h"
#include "ast.h"
#include "token.h"

struct MDDoc* md_doc_init() {
    return malloc(sizeof(MDDoc));
}

void md_doc_dump(const MDDoc* self) {
    if (self->source)
        nc_string_dump(self->source);
    if (self->tokens)
        token_list_dump(self->tokens);
    if (self->tree)
        ast_dump(self->tree);
}

void md_doc_free(MDDoc* self) {
    if (self->source)
        nc_string_free(self->source);
    if (self->tokens)
        token_list_free(self->tokens);
    if (self->tree)
        ast_free(self->tree);
    free(self);
    self = NULL;
}

