#include "logger.h"

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

static const char* level_name(LogLevel lvl)
{
    switch (lvl)
    {
    case LOG_ERROR:
        return "ERROR";
    case LOG_WARN:
        return "WARN";
    case LOG_INFO:
        return "INFO";
    case LOG_DEBUG:
        return "DEBUG";
    case LOG_TRACE:
        return "TRACE";
    default:
        return "UNKNOWN";
    }
}

static const char* level_color(LogLevel lvl)
{
    // ANSI colors (optional)
    switch (lvl)
    {
    case LOG_ERROR:
        return "\x1b[31m"; // red
    case LOG_WARN:
        return "\x1b[33m"; // yellow
    case LOG_INFO:
        return "\x1b[32m"; // green
    case LOG_DEBUG:
        return "\x1b[36m"; // cyan
    case LOG_TRACE:
        return "\x1b[90m"; // gray
    default:
        return "\x1b[0m";
    }
}

void log_init(Logger* log, FILE* out, LogLevel level)
{
    if (!log)
        return;
    log->level = level;
    log->out = out ? out : stderr;
    log->use_color = false;
}

void log_set_level(Logger* log, LogLevel level)
{
    if (!log)
        return;
    log->level = level;
}

void log_set_color(Logger* log, bool enabled)
{
    if (!log)
        return;
    log->use_color = enabled;
}

void log_vmsg(Logger* log, LogLevel lvl, const char* fmt, va_list args)
{
    if (!log || !fmt) return;
    if (lvl > log->level) return;

    FILE* out = log->out ? log->out : stderr;

    if (log->use_color) {
        fprintf(out, "%s[%s]%s ", level_color(lvl), level_name(lvl), "\x1b[0m");
    } else {
        fprintf(out, "[%s] ", level_name(lvl));
    }

    vfprintf(out, fmt, args);
    fputc('\n', out);
    fflush(out);
}

void log_msg(Logger* log, LogLevel lvl, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_vmsg(log, lvl, fmt, args);
    va_end(args);
}
