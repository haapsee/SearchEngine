#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

// Split a string into words. Returns a NULL-terminated array of strings. Caller must free.
char **utils_split_words(const char *text);

// Free a NULL-terminated array of strings
void utils_free_string_array(char **array);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
