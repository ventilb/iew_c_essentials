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
#ifndef MACROS_H
#define MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "col_error.h"
#include "utils.h"

#define makeVecApi(type) \
    size_t vec_##type##_len(vec_##type v); \
    col_error_t vec_##type##_pop_back(vec_##type v, type* res); \
    void vec_##type##_clear(vec_##type v); \
    int vec_##type##_empty(vec_##type v);  \
    type* vec_##type##_data(vec_##type v);

#define makePrimitiveVecOfTypeApi(type) \
    typedef type* iter_##type;         \
    struct vec__##type {                \
        size_t len;                     \
        size_t cap;                     \
        type *data;                     \
        type *begin;                    \
        type *end;                      \
    };                                  \
    typedef struct vec__##type* vec_##type; \
    vec_##type vec_##type##_new();      \
    col_error_t vec_##type##_free(vec_##type v); \
    col_error_t vec_##type##_reserve(vec_##type v, size_t new_cap); \
    col_error_t vec_##type##_push_back(vec_##type v, type val); \
    col_error_t vec_##type##_insert(vec_##type v, size_t i, type val); \
    col_error_t vec_##type##_back(vec_##type v, type* res);     \
    col_error_t vec_##type##_get(vec_##type v, size_t i, type* res); \
    col_error_t vec_##type##_set(vec_##type v, size_t i, type val);  \
    col_error_t vec_##type##_erase(vec_##type v, size_t i);                                   \
    iter_##type vec_##type##_begin(vec_##type v);   \
    iter_##type vec_##type##_end(vec_##type v);


#define makeVecImpl(type) \
    size_t vec_##type##_len(vec_##type v) { \
        return v->len;    \
    }                     \
    col_error_t vec_##type##_pop_back(vec_##type v, type* res) { \
        if (v->len <= 0) {\
            return COL_ERR_UNDERFLOW;       \
        }                 \
        v->len --;        \
        if (res != NULL) {\
            *res = v->data[v->len];         \
        }                 \
        v->end = NULL;    \
        return COL_OK;    \
    }                     \
    void vec_##type##_clear(vec_##type v) { \
        v->len = 0;       \
        v->end = NULL;    \
    }                     \
    int vec_##type##_empty(vec_##type v) { \
        return v->len == 0;                 \
    }                     \
    type* vec_##type##_data(vec_##type v) { \
        return v->data;   \
    }

#define makePrimitiveVecOfTypeImpl(type) \
vec_##type vec_##type##_new() {          \
    ltrace("[vec_new] - sizeof=%d", sizeof(struct vec__##type)); \
    vec_##type v = IEW_FN_ALIGNED_ALLOC(sizeof(void *), sizeof(struct vec__##type));    \
    if (v) {                             \
        v->len = 0;                      \
        v->cap = 0;                      \
        v->data = NULL;                  \
        v->begin = NULL;                 \
        v->end = NULL;                   \
    }                                    \
    return v;                            \
} \
                                         \
col_error_t vec_##type##_free(vec_##type v) {                    \
        if (v) {                         \
            if (v->data) {IEW_FN_FREE(v->data);}                 \
            v->len = 0;                  \
            v->cap = 0;                  \
            v->data = NULL;              \
            v->begin = NULL;             \
            v->end = NULL;               \
            IEW_FN_FREE(v);              \
        }                                \
        return COL_OK;                   \
    }                                    \
                                         \
col_error_t vec_##type##_reserve(vec_##type v, size_t new_cap) { \
    ltrace("[vec_reserve] - requested new cap=%d", new_cap);     \
    if (new_cap <= v->cap) {             \
        ltrace("[vec_reserve] - vector has space, no resize");   \
        return COL_OK;                   \
    }                                    \
                                         \
    new_cap = (size_t) ceil(VEC_GROWTH * (double) new_cap);      \
    ltrace("[vec_reserve] - adjusted new cap=%d", new_cap);      \
    if (new_cap > SIZE_MAX / sizeof(type)) {                     \
        return COL_ERR_BAD_ALLOC;        \
    }                                    \
                                         \
    size_t size_bytes = new_cap * sizeof(type);                  \
    const size_t alignment = sizeof(void *);                     \
                                         \
    ltrace("[vec_reserve] - currentCap=%d, len=%d, new cap=%d", v->cap, v->len, new_cap); \
    void * data = col_align_alloc(v->data, size_bytes, alignment);\
                                         \
    if (data == NULL) {                  \
        return COL_ERR_BAD_ALLOC;        \
    }                                    \
    v->data = data;                      \
    v->cap = new_cap;                    \
    v->begin = NULL;                     \
    v->end = NULL;                       \
    return COL_OK;                       \
}                                        \
                                         \
col_error_t vec_##type##_push_back(vec_##type v, type val) {     \
    ltrace("[vec_push_back] - begin=%p, begin1=%p", v->begin, v->begin);                  \
    col_error_t err = COL_OK;            \
    if ((err = vec_##type##_reserve(v, v->len + 1)) != COL_OK) { \
        return err;                      \
    }                                    \
                                         \
    ltrace("[push_back] - len=%d", v->len);                      \
    v->data[v->len] = val;               \
    ltrace("[push_back] - data=%p, data[0]=%d", v->data, *v->data);                       \
    v->len ++;                           \
    return COL_OK;                       \
}                                        \
                                         \
col_error_t vec_##type##_insert(vec_##type v, size_t i, type val) {                       \
    col_error_t err = COL_OK;            \
    if ((err = vec_##type##_reserve(v, v->len + 1)) != COL_OK) { \
        return err;                      \
    }                                    \
    if (i < v->len) {                    \
        void *dest = memmove(v->data + i + 1, v->data + i, (v->len - i) * sizeof(type));  \
        if (dest != v->data+i + 1) {     \
            return COL_ERR_BAD_ALLOC;    \
        }                                \
    } else {                             \
        i = v->len;                      \
    }                                    \
    v->data[i] = val;                    \
    v->len ++;                           \
    return COL_OK;                       \
}                                        \
                                         \
col_error_t vec_##type##_back(vec_##type v, type* res) {         \
    if (v->len <= 0) {                   \
        return COL_ERR_UNDERFLOW;        \
    }                                    \
    *res = v->data[v->len-1];            \
    return COL_OK;                       \
}                                        \
                                         \
col_error_t vec_##type##_get(vec_##type v, size_t i, type* res) {\
    if (i >= v->len) {                   \
        return COL_ERR_ILLEGAL_ARGUMENT; \
    }                                    \
    *res = v->data[i];                   \
    return COL_OK;                       \
}                                        \
                                         \
col_error_t vec_##type##_set(vec_##type v, size_t i, type val) { \
    if (i >= v->len) {                   \
        return COL_ERR_ILLEGAL_ARGUMENT; \
    }                                    \
    v->data[i] = val;                    \
    return COL_OK;                       \
}                                        \
                                         \
col_error_t vec_##type##_erase(vec_##type v, size_t i) {   \
    if (i >= v->len) {                   \
        return COL_ERR_ILLEGAL_ARGUMENT; \
    }                                    \
    void *dest = memmove(v->data + i, v->data + i + 1, (v->len - i) * sizeof(type));\
    if (dest != v->data+i) {             \
        return COL_ERR_BAD_ALLOC;        \
    }                                    \
    v->len --;                           \
    v->end = NULL;                       \
    return COL_OK;                       \
}                                        \
                                         \
iter_##type vec_##type##_begin(vec_##type v) {                   \
        if (!v->begin) v->begin = v->data;                       \
        return v->begin;                 \
    }                                    \
                                         \
iter_##type vec_##type##_end(vec_##type v) {                     \
        if (!v->end) v->end = v->data + v->len;                  \
        return v->end;                   \
    }


#ifdef __cplusplus
};
#endif

#endif // MACROS_H