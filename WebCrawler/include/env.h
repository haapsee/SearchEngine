#ifndef ENV_H
#define ENV_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EnvironmentVariable {
    char* key;
    char* value;
    struct EnvironmentVariable* next;
} EnvironmentVariable;

// Load environment variables from a file (e.g., .env)
int env_load(const char* filepath);

// Get the value of an environment variable
// Returns NULL if not found. If not NULL, the returned pointer is valid until next call.
const char* env_get(const char* key, const char* default_value);

// Set or override an environment variable
void env_set( char* key,  char* value);

// Free all loaded environment variables
void env_free();

#ifdef __cplusplus
}
#endif

#endif // ENV_H
