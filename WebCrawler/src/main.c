#include <stdio.h>
#include "../include/env.h"
#include "../include/db_manager.h"
#include "../include/html_parser.h"

int main() {
    const char* html = "href=\"abc\" href='def'";
    char** links = html_parser_extract_links(html, "example.com");

    int c = 0;
    while (links[c] != NULL) {
        printf("%s\n", links[c]);
        c++;
    }

    return 0;
}
