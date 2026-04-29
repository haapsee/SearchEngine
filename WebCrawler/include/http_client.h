#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

// Fetch the content of a URL. Returns a newly allocated string with the response body. Caller must free.
char *http_client_fetch(const char *url, long *http_code);

#ifdef __cplusplus
}
#endif

#endif // HTTP_CLIENT_H
