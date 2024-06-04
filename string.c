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

#include "string.h"
#include "utils.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>

char * str_of(const char * s) {
    return str_nbof(s, SIZE_MAX);
}

char * str_nbof(const char * s, size_t nb) {
    if (s == NULL || nb == 0) {
        return str_of_empty();
    }
    size_t slen_bytes = utf8size_lazy(s);
    if (nb > slen_bytes) {
        nb = slen_bytes;
    }

    if (nb == 0) {
        // Nothing to add
        return str_of_empty();
    } else {
        char * buf;
        if ((buf = col_align_alloc(NULL, nb + 1, sizeof(void *))) == NULL) {
            return NULL;
        }
        memcpy(buf, s, nb);
        buf[nb] = '\0';

        return buf;
    }
}

char * str_of_empty() {
    char * buf;
    if ((buf = col_align_alloc(NULL, 1, sizeof(void *))) == NULL) {
        return NULL;
    }
    buf[0] = '\0';
    return buf;
}

char * str_free(char * str) {
    if (str != NULL) {
        IEW_FN_FREE(str);
    }
    return NULL;
}

size_t str_len(const char * str) {
    return utf8len(str);
}

size_t str_size(const char * str) {
    return utf8size_lazy(str);
}

char * str_append(char * str, const char * s) {
    if (s == NULL) {
        return str;
    }
    const size_t slen_bytes = utf8size_lazy(s);
    if (slen_bytes == 0) {
        // Nothing to add
        return str;
    }

    char * buf;
    if (str == NULL) {
        buf = str_of(s);
    } else {
        const size_t strlen_bytes = utf8size_lazy(str);
        if ((buf = col_align_alloc(str, strlen_bytes + slen_bytes + 1, sizeof(void *))) == NULL) {
            return NULL;
        }
        memcpy(buf, str, strlen_bytes);
        memcpy(buf + strlen_bytes, s, slen_bytes);
        buf[strlen_bytes + slen_bytes] = '\0';
    }

    return buf;
}

// https://github.com/sheredom/utf8.h
// We had to copy and adjust the code because
// utf8.h 'utf8chr' function for example returns
// pointers, but we want to work with codepoint indices.
void str_rune_to_chars(int rune, char chars[5]) {
    chars[0] = '\0';
    chars[1] = '\0';
    chars[2] = '\0';
    chars[3] = '\0';
    chars[4] = '\0';

    if (0 == rune) {
        return;
    } else if (0 == ((utf8_int32_t)0xffffff80 & rune)) {
        /* 1-byte/7-bit ascii
         * (0b0xxxxxxx) */
        chars[0] = (utf8_int8_t)rune;
    } else if (0 == ((utf8_int32_t)0xfffff800 & rune)) {
        /* 2-byte/11-bit utf8 code point
         * (0b110xxxxx 0b10xxxxxx) */
        chars[0] = (utf8_int8_t)(0xc0 | (utf8_int8_t)(rune >> 6));
        chars[1] = (utf8_int8_t)(0x80 | (utf8_int8_t)(rune & 0x3f));
    } else if (0 == ((utf8_int32_t)0xffff0000 & rune)) {
        /* 3-byte/16-bit utf8 code point
         * (0b1110xxxx 0b10xxxxxx 0b10xxxxxx) */
        chars[0] = (utf8_int8_t)(0xe0 | (utf8_int8_t)(rune >> 12));
        chars[1] = (utf8_int8_t)(0x80 | (utf8_int8_t)((rune >> 6) & 0x3f));
        chars[2] = (utf8_int8_t)(0x80 | (utf8_int8_t)(rune & 0x3f));
    } else { /* if (0 == ((int)0xffe00000 & chr)) { */
        /* 4-byte/21-bit utf8 code point
         * (0b11110xxx 0b10xxxxxx 0b10xxxxxx 0b10xxxxxx) */
        chars[0] = (utf8_int8_t)(0xf0 | (utf8_int8_t)(rune >> 18));
        chars[1] = (utf8_int8_t)(0x80 | (utf8_int8_t)((rune >> 12) & 0x3f));
        chars[2] = (utf8_int8_t)(0x80 | (utf8_int8_t)((rune >> 6) & 0x3f));
        chars[3] = (utf8_int8_t)(0x80 | (utf8_int8_t)(rune & 0x3f));
    }
}

int str_index_rune(const char * str, int chr) {
    if (chr == 0) {
        return 0;
    }
    utf8_int8_t c[5] = {'\0', '\0', '\0', '\0', '\0'};

    str_rune_to_chars(chr, c);

    return str_index_string(str, c);
}

// Code adapted from: https://github.com/sheredom/utf8.h
// We had to copy and adjust the code because
// utf8.h 'utf8chr' function for example returns
// pointers, but we want to work with codepoint indices.
int str_index_string(const char * str, const char * lookup) {
    if (str == NULL || lookup == NULL) {
        return -1;
    }
    utf8_int32_t throwaway_codepoint = 0;

    /*
     * if needle has no utf8 codepoints before the null terminating
     * byte then return 0.
     * Empty string is always in a string.
     */
    if ('\0' == *lookup) {
        return 0;
    }

    int codepoints_count = 0;
    while ('\0' != *str) {
        const utf8_int8_t *maybeMatch = str;
        const utf8_int8_t *n = lookup;

        // Match bytes of string and lookup string
        while (*str == *n && (*str != '\0' && *n != '\0')) {
            n++;
            str++;
        }

        if ('\0' == *n) {
            return codepoints_count;
        } else {
            /* h could be in the middle of an unmatching utf8 codepoint,
             * so we need to march it on to the next character beginning
             * starting from the current character */
            str = utf8codepoint(maybeMatch, &throwaway_codepoint);
            codepoints_count ++;
        }
    }

    return -1;
}

typedef struct str_scan_token_t {
    size_t scan_index;
    char * str_mem;
    size_t codepoint_index;
} str_scan_token;

// Internal utility for substring and other codepoint index based functions.
// Scan the string and gather information about the string.
void str_codepoint_byte_pos(char * str, str_scan_token * tok_start, str_scan_token * tok_end) {
    if (str == NULL) {
        return;
    }
    utf8_int32_t throwaway_codepoint = 0;

    /*
     * if needle has no utf8 codepoints before the null terminating
     * byte then return 0.
     * Empty string is always in a string.
     */
    if (tok_start->scan_index == 0) {
        tok_start->str_mem = str;
        tok_start->codepoint_index = 0;
    }
    if (tok_end->scan_index == 0) {
        tok_end->str_mem = str;
        tok_end->codepoint_index = 0;
    }

    size_t codepoints_count = 0;
    while ('\0' != *str) {
        utf8_int8_t *maybeMatch = str;

        if (codepoints_count == tok_start->scan_index) {
            tok_start->str_mem = maybeMatch;
            tok_start->codepoint_index = codepoints_count;
        }
        if (codepoints_count == tok_end->scan_index) {
            tok_end->str_mem = maybeMatch;
            tok_end->codepoint_index = codepoints_count;
        }
        /* h could be in the middle of an unmatching utf8 codepoint,
         * so we need to march it on to the next character beginning
         * starting from the current character */
        str = utf8codepoint(maybeMatch, &throwaway_codepoint);
        codepoints_count ++;
    }

    if (codepoints_count <= tok_start->scan_index) {
        tok_start->str_mem = str;
        tok_start->codepoint_index = codepoints_count;
    }
    if (codepoints_count <= tok_end->scan_index) {
        tok_end->str_mem = str;
        tok_end->codepoint_index = codepoints_count;
    }
}

char * str_substring(const char * str, size_t start, size_t end) {
    // Same index return the empty string
    if (start == end) {
        return str_of_empty();
    }

    // Fix order of indices
    if (start > end) {
        size_t tmp = end;
        end = start;
        start = tmp;
    }

    // Scan the string once and resolve the byte positions for the
    // codepoint indices
    str_scan_token tok_start = {.scan_index = start, .str_mem = NULL, .codepoint_index = 0};
    str_scan_token tok_end = {.scan_index = end, .str_mem = NULL, .codepoint_index = 0};

    str_codepoint_byte_pos((char *) str, &tok_start, &tok_end);

    return str_nbof(tok_start.str_mem, (tok_end.str_mem - tok_start.str_mem));
}

