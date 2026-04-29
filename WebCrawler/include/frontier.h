#ifndef FRONTIER_H
#define FRONTIER_H

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the URL frontier
void frontier_init();

// Add a URL to the frontier
void frontier_enqueue(const char *url);

// Get the next URL from the frontier. Returns NULL if empty. Caller must free.
char *frontier_dequeue();

// Check if the frontier is empty
int frontier_is_empty();

// Free all resources used by the frontier
void frontier_free();

#ifdef __cplusplus
}
#endif

#endif // FRONTIER_H
