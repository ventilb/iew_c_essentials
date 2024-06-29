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

#ifndef IEW_C_ESSENTIALS_BUF_MACROS_H
#define IEW_C_ESSENTIALS_BUF_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "col_error.h"
#include "icelogging.h"
#include "icemalloc.h"
/**
 * buf_<name>_memset
 * Uses memset which is not safe to erase data because
 * the memset call might be optimized away
 *
 * @brief buf_<name>_get(buf_<name> v, size_t i, <type>** res)
 * Fetch buffer entry at position i and put pointer to entry
 * at res. Returns COL_OK on success or COL_ERR_UNDERFLOW if
 * i is greater or equal to the buffers limit.
 */

#define makeBufOfTypeApi(name, type) \
    struct buf__##name {             \
        size_t lim;                  \
        size_t cap;                  \
        size_t align;                \
        size_t alignedSize;          \
        char* data;                  \
    };                               \
    typedef struct buf__##name* buf_##name; \
    typedef col_error_t (*PFN_buf_##name##_each)(buf_##name v, size_t i, void * pUserData); \
    typedef col_error_t (*PFN_buf_##name##_pred)(buf_##name v, size_t i, bool * pMatch, void * pUserData); \
    buf_##name buf_##name##_new(size_t align);   \
    void buf_##name##_free(buf_##name v);                                            \
    col_error_t buf_##name##_reserve(buf_##name v, size_t new_cap);                         \
    col_error_t buf_##name##_back(buf_##name v, type** res);                                \
    col_error_t buf_##name##_emplace_back(buf_##name v, type** res);                        \
    col_error_t buf_##name##_get(buf_##name v, size_t i, type** res);                       \
    col_error_t buf_##name##_memset(buf_##name v, size_t i, int c);                         \
    size_t buf_##name##_lim(buf_##name v);  \
    void buf_##name##_set_lim(buf_##name v, size_t lim);                                    \
    void buf_##name##_clear(buf_##name v);  \
    int buf_##name##_empty(buf_##name v);   \
    type* buf_##name##_data(buf_##name v);  \
    col_error_t buf_##name##_each(buf_##name v, PFN_buf_##name##_each cb, void * pUserData);\
    col_error_t buf_##name##_each_reverse(buf_##name v, PFN_buf_##name##_each each, void * pUserData); \
    col_error_t buf_##name##_search(buf_##name v, PFN_buf_##name##_pred predicate, size_t *pIndex, void * pUserData);

#define makeBufOfTypeImpl(name, type) \
buf_##name buf_##name##_new(size_t align) { \
    ltrace("[buf_new] - sizeof=%d, sizeof type=%d", sizeof(struct buf__##name), sizeof(type)); \
    buf_##name v = (buf_##name) ice_aligned_malloc(align, sizeof(struct buf__##name)); \
    if (v) {                          \
        v->lim = 0;                   \
        v->cap = 0;                   \
        v->align = align;             \
        v->alignedSize = ice_align_up(sizeof(type), align);\
        v->data = NULL;               \
    }                                 \
    return v;                         \
}                                     \
                                      \
void buf_##name##_free(buf_##name v) {                    \
        if (v) {                      \
            if (v->data) {ice_aligned_free(v->data);}            \
            v->lim = 0;               \
            v->cap = 0;               \
            v->align = 0;             \
            v->alignedSize = 0;       \
            v->data = NULL;           \
            ice_aligned_free(v);      \
        }                             \
    }                                 \
                                      \
col_error_t buf_##name##_reserve(buf_##name v, size_t new_cap) { \
    ltrace("[buf_reserve] - requested new cap=%d", new_cap);     \
    if (new_cap <= v->cap) {          \
        ltrace0("[buf_reserve] - vector has space, no resize");   \
        return COL_OK;                \
    }                                 \
                                      \
    new_cap = (size_t) ceil(VEC_GROWTH * (double) new_cap);      \
    ltrace("[buf_reserve] - adjusted new cap=%d", new_cap);      \
    if (new_cap > SIZE_MAX / sizeof(type)) {                     \
        return COL_ERR_BAD_ALLOC;     \
    }                                 \
                                      \
    const size_t old_size_bytes = v->cap * v->alignedSize;                                     \
    const size_t new_size_bytes = new_cap * v->alignedSize;                                    \
                                      \
    ltrace("[buf_reserve] - cur_cap=%ld, lim=%ld, new_cap=%ld, aligned_size=%ld, old_size_bytes=%ld, new_size_bytes=%ld", v->cap, v->lim, new_cap, v->alignedSize, old_size_bytes, new_size_bytes); \
    char * data = (char *) ice_aligned_realloc(v->data, v->align, old_size_bytes, new_size_bytes); \
                                         \
    if (data == NULL) {                  \
        return COL_ERR_BAD_ALLOC;        \
    }                                    \
    v->data = data;                      \
    v->cap = new_cap;                    \
    return COL_OK;                       \
}                                        \
                                         \
col_error_t buf_##name##_back(buf_##name v, type** res) {         \
    if (v->lim <= 0) {                   \
        return COL_ERR_UNDERFLOW;        \
    }                                    \
    *res = (type *) (v->data + ((v->lim-1) * v->alignedSize));   \
    return COL_OK;                    \
}                                     \
                                      \
col_error_t buf_##name##_emplace_back(buf_##name v, type** res) {\
    col_error_t err = buf_##name##_reserve(v, v->lim + 1);       \
    if (err != COL_OK) {              \
        return err;                   \
    }                                 \
    buf_##name##_set_lim(v, v->lim + 1);    \
    return buf_##name##_get(v, v->lim - 1, res);\
}                                     \
                                      \
col_error_t buf_##name##_get(buf_##name v, size_t i, type** res) {\
    if (i >= v->lim) {                \
        return COL_ERR_OVERFLOW;      \
    }                                 \
    *res = (type *) (v->data + (i * v->alignedSize));            \
    return COL_OK;                    \
}                                     \
                                      \
col_error_t buf_##name##_memset(buf_##name v, size_t i, int c) {                                         \
    if (i >= v->lim) {                \
        return COL_ERR_OVERFLOW;      \
    }                                 \
    void * dest = memset((v->data + (i * v->alignedSize)), c, v->alignedSize);           \
    if (dest != (v->data + (i * v->alignedSize))) {                \
        return COL_ERR_BAD_ALLOC;     \
    }                                 \
    return COL_OK;                    \
}                                     \
                                      \
size_t buf_##name##_lim(buf_##name v) {        \
    return v->lim;                             \
}                                              \
void buf_##name##_set_lim(buf_##name v, size_t lim) {                                      \
    if (lim >= v->cap) {              \
        lim = v->cap;                 \
    }                                 \
    v->lim = lim;                     \
}                                     \
void buf_##name##_clear(buf_##name v) {        \
    v->lim = 0;                                \
}                                              \
int buf_##name##_empty(buf_##name v) {         \
    return v->lim == 0;                        \
}                                              \
type* buf_##name##_data(buf_##name v) {        \
    return (type *) v->data;          \
}                                              \
                                               \
col_error_t buf_##name##_each(buf_##name v, PFN_buf_##name##_each cb, void * pUserData) {                     \
    col_error_t err = COL_OK;         \
    for (size_t i = 0; i < v->lim; ++i) {   \
        err = cb(v, i, pUserData);    \
        if (err != COL_OK) {          \
            return err;               \
        }                             \
    }                                 \
    return err;                       \
}                                     \
col_error_t buf_##name##_each_reverse(buf_##name v, PFN_buf_##name##_each cb, void * pUserData) {               \
    col_error_t err = COL_OK;         \
    /* size_t cannot become negative. So
     * we must call callback for the first
     * element extra */               \
    for (size_t i = v->lim - 1; i > 0; --i) {                    \
        err = cb(v, i, pUserData);    \
        if (err != COL_OK) {          \
            return err;               \
        }                             \
    }                                 \
    return cb(v, 0, pUserData);       \
} \
col_error_t buf_##name##_search(buf_##name v,                    \
                                PFN_buf_##name##_pred predicate, \
                                size_t *pIndex,                  \
                                void * pUserData) {              \
    col_error_t err = COL_OK;         \
    bool match = false;               \
    for (size_t i = 0; i < v->lim; ++i) {   \
        err = predicate(v, i, &match, pUserData);                \
        if (err != COL_OK) {          \
            return err;               \
        } else if (match) {           \
            *pIndex = i;              \
            return err;               \
        }                             \
    }                                 \
    *pIndex = v->lim;                 \
    return err;                       \
}

#ifdef __cplusplus
};
#endif

#endif //IEW_C_ESSENTIALS_BUF_MACROS_H
