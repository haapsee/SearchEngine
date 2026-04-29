#define _GNU_SOURCE
#include <string.h>
#include "../include/env.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>


int main() {
    printf("Testing env.c\n");
    fflush(stdout);

    printf("Create in memory file\n");
    fflush(stdout);
    int fd = memfd_create("mem_file", 0);

    printf("Writing env file in memory");
    fflush(stdout);
    char* environ_str = "ENV1=ABC\nENV2=DEF\n";
    write(fd, environ_str, strlen(environ_str));

    printf("create filepath\n");
    fflush(stdout);
    char filepath[64];
    sprintf(filepath, "/proc/self/fd/%d", fd);

    printf("Reading environment variables from %s\n", filepath);
    fflush(stdout);

    env_load(filepath);

    printf("Testing env_load\n");
    fflush(stdout);
    const char* env1 = env_get("ENV1", "ERR");
    printf("ENV1 = %s, %d\n", env1,  strcmp(env1, "ABC"));
    fflush(stdout);
    assert(strcmp(env1, "ABC")==0);

    printf("Test wrong filepath\n");
    fflush(stdout);
    sprintf(filepath, "/proc/self/fd/-%d", fd);
    assert(env_load(filepath)==1);

    printf("\nAll tests passed for env.c\n\n");
    fflush(stdout);
    return 0;
}
