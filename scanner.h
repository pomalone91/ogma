#ifndef scanner_h
#define scanner_h

#include <stdio.h>
#include "../lib/nc_string.h"
#include "token.h"

// Scanner 
typedef struct Scanner {
    NCString* source;
    TokenList* tokens;
} Scanner;

struct Scanner* scanner_init();
void scanner_scan(Scanner* self);
void scanner_dump(const Scanner* self);
void scanner_free(Scanner* self);


#endif /*scanner*/
