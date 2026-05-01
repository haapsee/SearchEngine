#include "../include/html_parser.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

typedef struct {
    regex_t http;
    regex_t domain;
    regex_t relative;
} URLCheckers;

void init_checkers(URLCheckers *c) {
    regcomp(&c->http, "^https?://", REG_EXTENDED | REG_ICASE | REG_NOSUB);
    regcomp(&c->domain, "^([a-z0-9]+(-[a-z0-9]+)*\\.)+[a-z]{2,}", REG_EXTENDED | REG_ICASE | REG_NOSUB);
    regcomp(&c->relative, "^(\\.\\.?\\/|\\/)", REG_EXTENDED | REG_NOSUB);
}

char *unused_prefix[] = {
    "mailto:",
    "tel:",
    "sms:",
    "whatsapp:",
    "skype:",
    "fax:",
    "geo:",
    "#",
    NULL
};

int is_invalid_prefix(char *url) {
    for (int i = 0; unused_prefix[i] != NULL; i++) {
        char *prefix = unused_prefix[i];
        int prefix_length = strlen(prefix);
        if (strncmp(prefix, url, prefix_length)==0) {
            return 1;
        }
    }
    return 0;
}

char *normalize_url(char *url, const char *base_url, URLCheckers *checkers) {
    if (regexec(&checkers->http, url, 0, NULL, 0) == 0) {
        if (strncmp(url, "http://", 7)) {
            return strdup(url) + 7;
        } else {
            return strdup(url) + 8;
        }
    } else if (regexec(&checkers->domain, url, 0, NULL, 0) == 0) {
        return url;
    } else  if (regexec(&checkers->relative, url, 0, NULL, 0) == 0) {
        if (url[0] == '/') {
            char *new_url = malloc(sizeof(char)*1024*1024);
            sprintf(new_url, "%s%s", base_url, url);
            char *result = strdup(new_url);
            free(new_url);
            return result;
        }
        return strdup(base_url); // THis will make it discarded for now
    } else {
        return strdup(base_url);
    }
}

char **html_parser_extract_links(const char *html, const char *base_url) {
    URLCheckers *checkers = malloc(sizeof(URLCheckers));
    init_checkers(checkers);

    char *html_copy = strdup(html);
    char **links = NULL;
    int links_count = 0;

    while (html_copy[0] != '\0') {
        if (strncmp(html_copy, "href=\"", 6) == 0 || strncmp(html_copy, "href='", 6) == 0) {
            for (size_t i = 6; i < strlen(html_copy); i++) {
                if (html_copy[i] == html_copy[5]) {
                    html_copy += 6;
                    char* link = strndup(html_copy, i-6);
                    if (is_invalid_prefix(link)) {
                        break;
                    }
                    char** new_links = malloc(sizeof(char*)*(links_count+1));
                    for (int i = 0; i < links_count; i++) {
                        new_links[i] = links[i];
                    }
                    free(links);
                    links = new_links;
                    new_links[links_count] = normalize_url(link, base_url, checkers);
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
    char *html_dup = strdup(html);
    char *text = (char*) malloc(sizeof(char)*1024*1024);

    int text_idx = 0;
    int whitespace = 0;
    int skip = 0;

    while (html_dup[0] != '\0') {
        char c = html_dup[0];
        if (c == '<') {
            if (whitespace==0) {
                whitespace = 1;
                text[text_idx++] = ' ';
            }
            skip = 1;
        } else if (skip == 1 && c == '>') {
            skip = 0;
        } else if (!(isspace(c) && whitespace) && !skip) {
            if (isspace(c)) {
                c = ' ';
                whitespace = 1;
            } else {
                whitespace = 0;
            }
            text[text_idx++] = c;
        }
        html_dup++;
    }

    char *result = strdup(text);
    free(text);
    return result;
}
