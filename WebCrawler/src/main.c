#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/env.h"
#include "../include/db_manager.h"
#include "../include/html_parser.h"
#include "../include/http_client.h"
#include "logger.h"

typedef struct Node {
    char *url;
    struct Node *next;
} Node;


char *extract_base_url(char *url) {
    char *dup = strdup(url);
    for ( size_t i = 0; i < strlen(dup); i++) {
        if (dup[i] == '/') {
            dup[i] = '\0';
            return dup;
        }
    }
    return dup;
}

struct Node *push_to_q(struct Node *last, char **links) {
    for (int i = 0; links[i] != NULL; i++) {
        struct Node *n = (struct Node*)malloc(sizeof(struct Node));
        n->next = NULL;
        n->url = links[i];
        if (last != NULL) {
            last->next = n;
        }
        last = n;
    }
    return last;
}

struct Node *pop(struct Node *n) {
    struct Node *next = n->next;
    free(n);
    return next;
}

int main() {
    struct Node *last = (struct Node*)malloc(sizeof(struct Node));
    last->next = NULL;
    last->url = "reddit.com";
    struct Node *first = (struct Node*)malloc(sizeof(struct Node));
    first->next = last;
    first->url = "wikipedia.org/static/apple-touch/wikipedia.png";


    env_load(".env");
    const char* db_host = env_get("POSTGRES_HOST", "localhost");
    const char* db_db = env_get("POSTGRES_DB", "database");
    const char* db_port = env_get("POSTGRES_PORT", "5432");
    const char* db_user = env_get("POSTGRES_USER", "admin");
    const char* db_password = env_get("POSTGRES_PASSWORD", "admin");

    db_manager_init(db_host, db_port, db_db, db_user, db_password);

    while(db_manager_count_sites() < 100) {
        char *url = first->url;
        log_info("\nHandling %s\n", url);
        long http_code = 0;
        log_debug("Fetching html");
        char *html = http_client_fetch(url, &http_code);
        log_debug("Paarsing content");
        char *content = html_parser_extract_text(html);
        log_debug("Parsing links");
        char **links = html_parser_extract_links(html, extract_base_url(url));
        if (links != NULL) {
            last = push_to_q(last, links);
        }
        first = pop(first);
        log_debug("Saving to database");
        db_manager_insert_site(url, content);
    }

    printf("\n\nProram finished!\n");
    fflush(stdout);
    return 0;
}
