#include "../include/env.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct EnvironmentVariable *environmentVariable = NULL;

const char* env_get(const char *key, const char *default_value) {
    struct EnvironmentVariable* current = environmentVariable;

    while (current!=NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return default_value;
}

void env_set(char *key, char *value) {
    if (environmentVariable == NULL) {
        environmentVariable = (EnvironmentVariable*)malloc(sizeof(EnvironmentVariable));
        environmentVariable->key = key;
        environmentVariable->value = value;
        environmentVariable->next = NULL;
        return ;
    }

    struct EnvironmentVariable *previous = NULL;
    struct EnvironmentVariable *current = environmentVariable;
    int exists = 0;

    while (current!=NULL&&exists==0) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            exists = 1;
        }
        previous = current;
        current = current->next;
    }

    if (exists == 0) {
        current = (struct EnvironmentVariable*) malloc(sizeof(struct EnvironmentVariable));
        current->key = key;
        current->value = value;
        current->next = NULL;
        previous->next = current;
    }
}

int env_load(const char *filepath) {
    FILE* file = fopen(filepath, "r");
    char buffer[1024];

    if (file == NULL) {
        char err[256];
        sprintf(err, "Error while reading environment variables from file: %s", filepath);
        perror(err);
        fflush(stdout);
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        char *separator = strchr(buffer, '=');
        if (!separator) { continue; }
        *separator = '\0';
        char *key = strdup(buffer);
        char *value = strdup(separator+1);
        if (value[strlen(value)-1] == '\n') {
            value[strlen(value)-1] = '\0';
        }
        env_set(key, value);
    }

    return 0;
}
