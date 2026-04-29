#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DEBUG,
    INFO,
    WARNING,
    ERROR
} LOG_LEVEL;

extern FILE* log_out;

void log_internal(const LOG_LEVEL level, const char* file, int line, const char* fmt, ...);

#define log_debug(fmt, ...) log_internal(DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) log_internal(INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) log_internal(WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) log_internal(ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_msg(level, fmt, ...) log_internal(level, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // LOGGER_H
