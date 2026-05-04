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
    if (strncmp(url, "//", 2) == 0) {
        printf("// - %s, %s\n", url, url+2);
        return strdup(url) + 2;
    }
    if (regexec(&checkers->http, url, 0, NULL, 0) == 0) {
        printf("Starts with http(s):// - %s\n", url);
        if (strncmp(url, "http://", strlen("http://"))==0) {
            return strdup(url) + strlen("http://");
        } else {
            return strdup(url) + strlen("https://");
        }
    } else if (regexec(&checkers->domain, url, 0, NULL, 0) == 0) {
        printf("Is domain - %s\n", url);
        return url;
    } else  if (regexec(&checkers->relative, url, 0, NULL, 0) == 0) {
        printf("Is relative - %s\n", url);
        if (url[0] == '/') {
            char *new_url = malloc(sizeof(char)*1024*1024);
            sprintf(new_url, "%s%s", base_url, url);
            char *result = strdup(new_url);
            free(new_url);
            return result;
        }
        return strdup(base_url); // THis will make it discarded for now
    } else {
        printf("Unknown: %s\n", url);
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
                    char** new_links = malloc(sizeof(char*)*(links_count+2));
                    for (int i = 0; i < links_count; i++) {
                        new_links[i] = links[i];
                    }
                    free(links);
                    links = new_links;
                    new_links[links_count] = normalize_url(link, base_url, checkers);
                    new_links[links_count+1] = NULL;
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

char *skip_style(char *html) {
    while (html[0] != '\0') {
        if (strncmp("</style>", html, strlen("</style>")) == 0) {
            html += strlen("</style>");
            return html;
        }
        html++;
    }
    return html;
}

char *skip_script(char *html) {
    while (html[0] != '\0') {
        if (strncmp("</script>", html, strlen("</script>")) == 0) {
            html += strlen("</script>");
            break;
        }
        html++;
    }
    return html;
}

char *skip_quotes(char *html) {
    while (html[0] != '\0') {
        if (html[0] == '\"') {
            html += 1;
            break;
        }
        html++;
    }
    return html;
}

char *skip_tag(char *html) {
    while (html[0] != '\0') {
        if (strncmp("=\"", html, 2) == 0) {
            html += 2;
            html = skip_quotes(html);
        } else if (html[0] == '>') {
            html++;
            break;
        } else {
            html++;
        }
    }
    return html;
}

char *html_parser_extract_text(const char *html) {
    char *html_dup = strdup(html);
    char *text = (char*) malloc(sizeof(char)*1024*1024);

    int text_idx = 0;
    int whitespace = 0;

    while (html_dup[0] != '\0') {
        char c = html_dup[0];
        if (c == '<') {
            if (strncmp("<script", html_dup, strlen("<script")) == 0) {
                html_dup = skip_script(html_dup);
            } else if (strncmp("<style", html_dup, strlen("<style")) == 0) {
                html_dup = skip_style(html_dup);
            } else {
                html_dup = skip_tag(html_dup);
            }
            if (!whitespace) {
                text[text_idx++] = ' ';
                whitespace = 1;
            }
            continue;
        } else if (!(isspace(c) && whitespace)) {
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
