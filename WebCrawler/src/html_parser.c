#include "../include/html_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char **html_parser_extract_links(const char *html, const char *base_url) {
    char *html_copy = strdup(html);
    char **links = NULL;
    int links_count = 0;

    while (html_copy[0] != '\0') {
        printf("HTML: %s\n", html_copy);
        if (strncmp(html_copy, "href=\"", 6) || strncmp(html_copy, "href='", 6)) {
            for (size_t i = 6; i < strlen(html_copy); i++) {
                if (html_copy[i] == html_copy[5]) {
                    html_copy += 6;
                    char* link = malloc(sizeof(char)*(i-5));
                    link = strndup(html_copy, i-6);
                    char** new_links = malloc(sizeof(char*)*(links_count+1));
                    for (int i = 0; i < links_count; i++) {
                        new_links[i] = links[i];
                    }
                    free(links);
                    links = new_links;
                    new_links[links_count] = link;
                    links_count++;
                    html_copy += i-6;
                    break;
                }
            }
        }
        html_copy++;
    }

    return links;
}

char *html_parser_extract_text(const char *html) {
    char *text = (char*) malloc(sizeof(char));
    return text;
}
