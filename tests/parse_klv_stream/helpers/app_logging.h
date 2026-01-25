#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/timeb.h>
#endif

/* ---------------------------
 * Log levels
 * --------------------------- */
typedef enum {
    LOG_INFO = 0,
    LOG_HIGHLIGHT,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_NONE
} log_level_t;

/* ---------------------------
 * Default log level (fallback)
 * Can be overridden in CMake or via environment variable
 * --------------------------- */
#ifndef GAPP_LOG_LEVEL
#define GAPP_LOG_LEVEL LOG_INFO
#endif

/* ---------------------------
 * ANSI color codes
 * --------------------------- */
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_WHITE   "\033[37m"
/* 256-color orange */
#define COLOR_ORANGE  "\033[38;5;208m"

/* ---------------------------
 * Timestamp buffer size
 * --------------------------- */
#define TIME_STR_LEN 32

/* ---------------------------
 * Get current time as string
 * --------------------------- */
static inline void get_time_str(char *buf)
{
#if defined(_WIN32)
    SYSTEMTIME st;
    GetLocalTime(&st);
    snprintf(buf, TIME_STR_LEN,
             "%04d-%02d-%02d %02d:%02d:%02d.%03d",
             st.wYear, st.wMonth, st.wDay,
             st.wHour, st.wMinute, st.wSecond,
             st.wMilliseconds);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm tm_info;
    localtime_r(&tv.tv_sec, &tm_info);
    strftime(buf, TIME_STR_LEN, "%Y-%m-%d %H:%M:%S", &tm_info);
    size_t len = strlen(buf);
    snprintf(buf + len, TIME_STR_LEN - len, ".%03d", (int)(tv.tv_usec / 1000));
#endif
}

/* ---------------------------
 * Determine effective log level at runtime
 * --------------------------- */
static inline log_level_t effective_log_level(void)
{
    const char *env = getenv("GAPP_LOG_LEVEL");
    if (!env || env[0] == '\0')
        return GAPP_LOG_LEVEL;

    if (!strcmp(env, "LOG_DEBUG"))     return LOG_DEBUG;
    if (!strcmp(env, "LOG_INFO"))      return LOG_INFO;
    if (!strcmp(env, "LOG_HIGHLIGHT")) return LOG_HIGHLIGHT;
    if (!strcmp(env, "LOG_WARNING"))   return LOG_WARNING;
    if (!strcmp(env, "LOG_ERROR"))     return LOG_ERROR;
    if (!strcmp(env, "LOG_NONE"))      return LOG_NONE;

    /* Invalid value → fallback */
    return GAPP_LOG_LEVEL;
}

/* ---------------------------
 * Internal logging function
 * --------------------------- */
static inline void log_log(log_level_t level,
                           const char *class_name,
                           const char *func,
                           int line,
                           const char *fmt, ...)
{
    log_level_t current = effective_log_level();
    if (current == LOG_NONE || level > current)
        return;

    char time_buf[TIME_STR_LEN];
    get_time_str(time_buf);

    const char *level_str = "UNKNOWN";
    const char *color = COLOR_WHITE;

    switch (level) {
        case LOG_DEBUG:
            level_str = "DEBUG";
            color = COLOR_GREEN;
            break;
        case LOG_INFO:
            level_str = "INFO";
            color = COLOR_WHITE;
            break;
        case LOG_HIGHLIGHT:
            level_str = "HIGHLIGHT";
            color = COLOR_ORANGE;
            break;
        case LOG_WARNING:
            level_str = "WARNING";
            color = COLOR_YELLOW;
            break;
        case LOG_ERROR:
            level_str = "ERROR";
            color = COLOR_RED;
            break;
        default:
            break;
    }

    if (class_name && class_name[0] != '\0') {
        fprintf(stderr, "%s[%s] [%s] [%s] %s():%d: ",
                color, time_buf, level_str, class_name, func, line);
    } else {
        fprintf(stderr, "%s[%s] [%s] %s():%d: ",
                color, time_buf, level_str, func, line);
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "%s\n", COLOR_RESET);
    fflush(stderr);
}

/* ---------------------------
 * Logging macros (no class)
 * --------------------------- */
#define LOG_DEBUG(fmt, ...) \
    do { log_log(LOG_DEBUG, NULL, __func__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#define LOG_INFO(fmt, ...) \
    do { log_log(LOG_INFO, NULL, __func__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#define LOG_HIGHLIGHT(fmt, ...) \
    do { log_log(LOG_HIGHLIGHT, NULL, __func__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#define LOG_WARNING(fmt, ...) \
    do { log_log(LOG_WARNING, NULL, __func__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#define LOG_ERROR(fmt, ...) \
    do { log_log(LOG_ERROR, NULL, __func__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

/* ---------------------------
 * Logging macros (with class)
 * --------------------------- */
#define LOG_DEBUG_CLASS(CLASS, fmt, ...) \
    do { log_log(LOG_DEBUG, CLASS, __func__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#define LOG_INFO_CLASS(CLASS, fmt, ...) \
    do { log_log(LOG_INFO, CLASS, __func__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#define LOG_HIGHLIGHT_CLASS(CLASS, fmt, ...) \
    do { log_log(LOG_HIGHLIGHT, CLASS, __func__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#define LOG_WARNING_CLASS(CLASS, fmt, ...) \
    do { log_log(LOG_WARNING, CLASS, __func__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#define LOG_ERROR_CLASS(CLASS, fmt, ...) \
    do { log_log(LOG_ERROR, CLASS, __func__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#endif /* _LOGGING_H_ */
