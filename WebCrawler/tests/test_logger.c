#include "../include/logger.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void verify_log_line(const char *line, const char *expected_level, const char *expected_message) {
    char date[20];
    char time[20];
    char level[20];
    char file[100];
    int line_num;
    char message[256];

    int result = sscanf(line, "%s %s [%[^]]] %[^:]:%d - %[^\n]",
                        date, time, level, file, &line_num, message);

    // 1. Check if all 6 fields were found
    assert(result == 6);

    // 2. Check the Level
    assert(strcmp(level, expected_level) == 0);

    // 3. Check the File
    assert(strcmp(file, "tests/test_logger.c") == 0);

    // 4. Check the Body
    assert(strcmp(message, expected_message) == 0);
}

void test_log_msg() {
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));

    // DEBUG
    printf("Testing log_msg DEBUG\n");
    fflush(stdout);
    FILE *mem_stream = fmemopen(buffer, sizeof(buffer), "w");
    log_out = mem_stream;
    log_msg(DEBUG, "This is %s %d", "Debug", 123);
    fclose(mem_stream);
    verify_log_line(buffer, "DEBUG", "This is Debug 123");

    // INFO
    printf("Passed...\n\nTesting log_msg INFO\n");
    fflush(stdout);
    mem_stream = fmemopen(buffer, sizeof(buffer), "w");
    log_out = mem_stream;
    log_msg(INFO, "This is %s %d", "Info", 123);
    fclose(mem_stream);
    verify_log_line(buffer, "INFO", "This is Info 123");

    // WARNING
    printf("Passed...\n\nTesting log_msg WARNING\n");
    fflush(stdout);
    mem_stream = fmemopen(buffer, sizeof(buffer), "w");
    log_out = mem_stream;
    log_msg(WARNING, "This is %s %d", "Warning", 123);
    fclose(mem_stream);
    verify_log_line(buffer, "WARNING", "This is Warning 123");

    // ERROR
    printf("Passed...\n\nTesting log_msg ERROR\n");
    fflush(stdout);
    mem_stream = fmemopen(buffer, sizeof(buffer), "w");
    log_out = mem_stream;
    log_msg(ERROR, "This is %s %d", "Error", 123);
    fclose(mem_stream);
    verify_log_line(buffer, "ERROR", "This is Error 123");

    printf("Passed...\n\n");
    fflush(stdout);
}

void test_log_debug() {
    printf("test_log_debug\n");

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    FILE *mem_stream = fmemopen(buffer, sizeof(buffer), "w");
    log_out = mem_stream;
    log_debug("This is %s", "Warning");
    fclose(mem_stream);

    printf("Passed...\n\n");
    fflush(stdout);
}

void test_log_info() {
    printf("test_log_info\n");

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    FILE *mem_stream = fmemopen(buffer, sizeof(buffer), "w");
    log_out = mem_stream;
    log_info("This is %s", "info");
    fclose(mem_stream);

    printf("Passed...\n\n");
    fflush(stdout);
}

void test_log_warning() {
    printf("test_log_warning\n");

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    FILE *mem_stream = fmemopen(buffer, sizeof(buffer), "w");
    log_out = mem_stream;
    log_warning("This is %s", "Warning");
    fclose(mem_stream);

    printf("Passed...\n\n");
    fflush(stdout);
}

void test_log_error() {
    printf("test_log_error\n");

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    FILE *mem_stream = fmemopen(buffer, sizeof(buffer), "w");
    log_out = mem_stream;
    log_error("This is %s", "Warning");
    fclose(mem_stream);

    printf("Passed...\n\n");
    fflush(stdout);
}

int main() {
    printf("Testing logger.c\n\n");
    fflush(stdout);

    test_log_msg();
    test_log_debug();
    test_log_info();
    test_log_warning();
    test_log_error();

    printf("\nAll tests passed!\n");
    fflush(stdout);
    return 0;
}
