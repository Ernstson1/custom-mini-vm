#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
    LOG_ERROR = 0,
    LOG_WARN = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3,
    LOG_TRACE = 4
} LogLevel;


typedef struct{
    LogLevel level;
    FILE* out;
    bool use_color; 
} Logger;


void log_init(Logger* log, FILE* out, LogLevel level);
void log_set_level(Logger* log, LogLevel level);
void log_set_color(Logger* log, bool enabled);

void log_msg(Logger* log, LogLevel lvl, const char* fmt, ...);
void log_vmsg(Logger* log, LogLevel lvl, const char* fmt, va_list args);

#endif // LOGGER_H
