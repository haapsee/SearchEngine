#ifndef URL_UTILS_H
#define URL_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

// Normalize a URL (remove fragments, resolve relative URLs, etc.).
// Returns a newly allocated string. Caller must free.
char *url_utils_normalize(const char *url, const char *base_url);

// Check if a URL is valid (http/https).
int url_utils_is_valid(const char *url);

#ifdef __cplusplus
}
#endif

#endif // URL_UTILS_H
