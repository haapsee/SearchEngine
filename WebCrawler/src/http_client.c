#include "../include/http_client.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdlib.h>
#include <string.h>

struct Memory {
    char *content;
    size_t size;
} Memory;

size_t write_html_call_back(void *data, size_t size, size_t nmemb, void *userptr) {
    size_t realsize = size * nmemb;
    struct Memory *result = (struct Memory *) userptr;

    char *ptr = realloc(result->content, result->size + realsize + 1);
    if (!ptr) return 0;

    result->content = ptr;
    memcpy(&(result->content[result->size]), data, realsize);
    result->size += realsize;
    result->content[result->size] = 0;

    return realsize;
}

char *http_client_fetch(const char *url, long *http_code) {
    CURL *curl = curl_easy_init();
    if (!curl) return NULL;

    struct Memory response = { .content = malloc(1), .size = 0 };

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_html_call_back);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    curl_easy_setopt(curl, CURLOPT_USERAGENT,
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
        "(KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36");

    int res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, http_code);
    }

    char *content = strdup(response.content);
    free(response.content);

    return content;
}


// ...implementation to be added...
