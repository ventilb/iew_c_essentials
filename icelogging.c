/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

#include "icelogging.h"

// https://stackoverflow.com/questions/6508461/logging-library-for-c

#if defined(IEW_LOG_LEVEL_ERROR) || defined(IEW_LOG_LEVEL_INFO) || defined(IEW_LOG_LEVEL_DEBUG) || defined(IEW_LOG_LEVEL_TRACE) || defined(IEW_MEM_TRACE)
void vlog_format(const char* level, const char* message, va_list args) {
    time_t now;
    time(&now);
    char * date =ctime(&now);
    date[strlen(date) - 1] = '\0';
    fprintf(stderr, "%s [%s] ", date, level);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
}
void log_format(const char* level, const char* message, ...) {
    va_list args;
    va_start(args, message);
    time_t now;
    time(&now);
    char * date =ctime(&now);
    date[strlen(date) - 1] = '\0';
    fprintf(stderr, "%s [%s] ", date, level);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);
}
#endif

#if defined(IEW_LOG_LEVEL_ERROR)
void log_error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    vlog_format("error", message, args);
    va_end(args);
}
#endif

#if defined(IEW_LOG_LEVEL_INFO)
void log_info(const char* message, ...) {
    va_list args;   va_start(args, message);
    vlog_format("info ", message, args);
    va_end(args);
}
#endif

#if defined(IEW_LOG_LEVEL_DEBUG)
void log_debug(const char* message, ...) {
    va_list args;
    va_start(args, message);
    vlog_format("debug", message, args);
    va_end(args);
}
#endif

#if defined(IEW_LOG_LEVEL_TRACE)
void log_trace(const char* message, ...) {
    va_list args;   va_start(args, message);
    vlog_format("trace", message, args);
    va_end(args);
}
void log_method_trace(const char* message, const char *const file, const int line) {
    log_format("trace", "[%s] - File: '%s', Line: '%d'", message, file, line);
}
#endif

#if defined(IEW_MEM_TRACE)
void log_mem_trace(const char* message, ...) {
    va_list args;
    va_start(args, message);
    vlog_format("  mem", message, args);
    va_end(args);
}
#endif
