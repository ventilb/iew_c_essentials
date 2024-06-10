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

#ifndef IEW_C_ESSENTIALS_ICELOGGING_H
#define IEW_C_ESSENTIALS_ICELOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#if defined(IEW_LOG_LEVEL_ERROR)
void log_error(const char* message, ...);
#define lerror0(m) lerror(m, "")
#define lerror(m, ...) { log_error(m, __VA_ARGS__); } while(0)
#else
#define lerror0(m)
#define lerror(m, ...)
#endif

#if defined(IEW_LOG_LEVEL_INFO)
void log_info(const char* message, ...);
#define linfo0(m) linfo(m, "")
#define linfo(m, ...) log_info(m, __VA_ARGS__)
#else
#define linfo0(m)
#define linfo(m, ...)
#endif

#if defined(IEW_LOG_LEVEL_DEBUG)
void log_debug(const char* message, ...);
#define ldebug0(m) ldebug(m, "")
#define ldebug(m, ...) log_debug(m, __VA_ARGS__)
#else
#define ldebug0(m)
#define ldebug(m, ...)
#endif

#if defined(IEW_LOG_LEVEL_TRACE)
void log_trace(const char* message, ...);
void log_method_trace(const char* message, const char *const file, const int line);
#define ltrace0(_m) ltrace(_m, "")
#define ltrace(_m, ...) log_trace(_m, __VA_ARGS__)
#define ctrace(_m) log_method_trace(_m, __FILE__, __LINE__)
#else
#define ltrace0(_m)
#define ltrace(_m, ...)
#define ctrace(_m)
#endif

#if defined(IEW_MEM_TRACE)
void log_mem_trace(const char* message, ...);
#define mem_trace0(m) mem_trace(m, "")
#define mem_trace(m, ...) log_mem_trace(m, __VA_ARGS__)
#else
#define mem_trace0(m)
#define mem_trace(m, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif // IEW_C_ESSENTIALS_ICELOGGING_H