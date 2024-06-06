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
#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <uchar.h>
#include "col_error.h"
#include <error.h>
#include "utf8.h"

/**
 * Create a clone of s. Returns the empty string if s is NULL.
 *
 * @param s The input string
 * @return Clone of s
 */
char * str_of(const char * s);

/**
 * Create a string from the first nb bytes from input string s. Returns the
 * empty string if s is NULL or nb is 0. nb is clamped to the byte length
 * of s.
 *
 * @param s The input string
 * @param nb Number of bytes excluding the '\0' byte
 * @return A new string consisting of the first nb bytes of s
 */
char * str_nbof(const char * s, size_t nb);

/**
 * Returns a new allocated empty string
 *
 * @return Empty string
 */
char * str_of_empty();

char * str_free(char * str);

/**
 * The length (number of codepoints) of str.
 *
 * @param str String
 * @return Length/Number of codepoints
 */
size_t str_len(const char * str);

/**
 * The number of bytes used by str without counting '\0'.
 *
 * @param str String
 * @return Number of bytes
 */
size_t str_size(const char * str);

/**
 * Append string s to string str. If s is NULL or empty string str is returned.
 * If str is NULL a clone of s is returned. Otherwise s is concatenated to str.
 *
 * The string s is not modified and can be safely used after str_append.
 * The memory of str might be reallocated. Using original pointer of str leads
 * to undefined behaviour.
 *
 * @param str Left string
 * @param s The string to append/concat
 * @return Concatenated string
 */
char * str_append(char * str, const char * s);

int str_index_rune(const char * str, int rune);

int str_index_string(const char * str, const char * lookup);

char * str_substring(const char * str, size_t start, size_t end);

#ifdef __cplusplus
};
#endif

#endif // STRING_BUILDER_H