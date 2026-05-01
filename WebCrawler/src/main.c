#include <stdio.h>
#include "../include/env.h"
#include "../include/db_manager.h"
#include "../include/html_parser.h"

int main() {
    const char *html = "<!-- Standard external link --><p><a href=\"https://www.google.com\" target=\"_blank\">Visit Google</a> (Opens in new tab)</p><!-- Internal/Relative link --><p><a href=\"/about-us\">About Us</a> (Links to a page on same server)</p><!-- Anchor link (Scrolls to a specific ID on the page) --><p><a href=\"#footer\">Jump to Footer</a></p><!-- Email link --><p><a href=\"mailto:support@example.com\">Contact Support</a></p><!-- Telephone link --><p><a href=\"tel:+15550109999\">Call Us: (555) 010-9999</a></p><!-- Download link --><p><a href=\"/files/whitepaper.pdf\" download>Download PDF</a></p><!-- Link as a Button (using CSS class) --><a href=\"/signup\" class=\"button-style\" style=\"padding: 10px; background: blue; color: white; text-decoration: none;\">Sign Up Now</a>";
    char** links = html_parser_extract_links(html, "example.com");

    int c = 0;
    while (links[c] != NULL) {
        printf("%s\n", links[c]);
        c++;
    }

    printf("\nContent:\n %s\n", html_parser_extract_text(html));

    printf("\n\nProram finished!\n");
    fflush(stdout);
    return 0;
}
