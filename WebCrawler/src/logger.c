#include "../include/logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

FILE* log_out = NULL;

const char* log_level_string[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR"
};

struct tm* logger_get_time() {
    time_t raw_time = time(NULL);
    return localtime(&raw_time);
}

const char* logger_level_to_string(const LOG_LEVEL level) {
    return log_level_string[level];
}

void log_internal(const LOG_LEVEL level, const char* file, int line, const char* fmt, ...) {
    FILE *out = log_out ? log_out : stdout;

    va_list args;
    va_start(args, fmt);
    char str_time[64];
    strftime(str_time, sizeof(str_time), "%Y-%m-%d %H:%M:%S", logger_get_time());
    fprintf(out, "%s [%s] %s:%d - ", str_time, logger_level_to_string(level), file, line);
    vfprintf(out, fmt, args);
    fprintf(out, "\n");
    va_end(args);
    fflush(out);
}
