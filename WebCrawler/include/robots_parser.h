#ifndef ROBOTS_PARSER_H
#define ROBOTS_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

// Parse robots.txt and check if a URL is allowed to crawl. Returns 1 if allowed, 0 if disallowed.
int robots_parser_is_allowed(const char *robots_txt, const char *user_agent, const char *url);

#ifdef __cplusplus
}
#endif

#endif // ROBOTS_PARSER_H
