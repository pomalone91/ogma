#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "token.h"
#include "token_list.h"
#include "../lib/nc_string.h"

int main (void) {

    //char *str = "# This is some markdown\n## Hell yeah. More text!\n### Header 3 baby!!!!";
    char *str = "# This *is* some markdown\n## Body\nLorem ipsum dolor sit amet, *consectetur* adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim **veniam**, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n";
    size_t len = strlen(str);
    NCString *string = nc_string_init(str, len);
    Scanner *s = scanner_init();
    s->source = string;

    scanner_scan(s);
    scanner_dump(s);

    scanner_free(s);

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
