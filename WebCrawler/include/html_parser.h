#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum HTMLElementType {
    HTML_TEXT,
    HTML_ELEMENT
} HTMLElementType;

typedef struct HTMLAttribute {
    char* name;
    char* value;
} HTMLAttribute;

typedef struct HTMLElement {
    char* name;
    char* content; // For text
    enum HTMLElementType type;
    struct HTMLAttribute* attributes;
    struct HTMLElement* parent;
    struct HTMLElement** children;  // For sub html elements
} HTMLElement;

struct HTMLElement* html_parser_parse(struct HTMLElement* element, char *html);

// Parse HTML content and extract all links (hrefs). Returns a NULL-terminated array of strings. Caller must free.
char **html_parser_extract_links(const char *html, const char *base_url);

// Parse HTML content and extract visible text. Returns a newly allocated string. Caller must free.
char *html_parser_extract_text(const char *html);

#ifdef __cplusplus
}
#endif

#endif // HTML_PARSER_H
