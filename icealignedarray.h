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

#ifndef IEW_C_ESSENTIALS_ICEALIGNEDARRAY_H
#define IEW_C_ESSENTIALS_ICEALIGNEDARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "col_error.h"
#include "utils.h"
#include "icelogging.h"
#include "icemalloc.h"

/**
 * The alignedarray set of functions are used for very special use cases.
 * In vulkan synchronization objects (VkFence, VkSemaphore,...) should be
 * pre allocated with fast read access.
 * The macros defined here create arrays where alignment and length are
 * specified at compile time.
 */

#define makeArrayOfTypeApi(name, type) \
    typedef type* arr_##name; \
    typedef col_error_t (*PFN_arr##name##each)(arr_##name v, size_t i, void * pUserData); \
    arr_##name arr_##name##_new();      \
    col_error_t arr_##name##_free(arr_##name v); \
    size_t arr_##name##_len(arr_##name v); \
    col_error_t arr_##name##_back(arr_##name v, type** res);     \
    col_error_t arr_##name##_get(arr_##name v, size_t i, type** res);                    \
    col_error_t arr_##name##_each(arr_##name v, PFN_arr##name##each cb, void * pUserData); \
    col_error_t arr_##name##_each_reverse(arr_##name v, PFN_arr##name##each cb, void * pUserData); \

#define makeArrayOfTypeImpl(name, type, align, len) \
arr_##name arr_##name##_new() {                     \
    const size_t size_aligned = (((size_t) sizeof(type) + align) & ~(align - 1)); \
    ltrace("[arr_new] - align=%ld, len=%ld, sizeof=%ld, size_aligned=%ld", align, len, sizeof(type), size_aligned); \
    arr_##name v = (arr_##name) ice_aligned_malloc(align, len * size_aligned); \
    if (v == NULL) {                                \
        return NULL;                                \
    }                                               \
    return v;                                       \
}                                                   \
                                                    \
col_error_t arr_##name##_free(arr_##name v) {       \
        if (v) {                                    \
            ice_aligned_free(v);                    \
        }                                           \
        return COL_OK;                              \
    }                                               \
size_t arr_##name##_len(arr_##name v) {             \
    return len;                                     \
}                                                   \
                                                    \
col_error_t arr_##name##_back(arr_##name v, type** res) {                           \
    return arr_##name##_get(v, len-1, res);         \
}                                                   \
                                                    \
col_error_t arr_##name##_get(arr_##name v, size_t i, type** res) {                  \
    if (i >= len) {                                 \
        return COL_ERR_ILLEGAL_ARGUMENT;            \
    }                                               \
    const size_t size_aligned = (((size_t) sizeof(type) + align) & ~(align - 1));   \
    ltrace("[arr_get] - i=%ld, size_aligned=%ld", i, size_aligned);                 \
    *res = (type *) (((char *) v) + i * size_aligned);                              \
    return COL_OK;                                  \
}                                                   \
col_error_t arr_##name##_each(arr_##name v, PFN_arr##name##each cb, void * pUserData) {                     \
    col_error_t err = COL_OK;                       \
    for (size_t i = 0; i < len; ++i) {              \
        err = cb(v, i, pUserData);                  \
        if (err != COL_OK) {                        \
            return err;                             \
        }                                           \
    }                                               \
    return err;                                     \
} \
col_error_t arr_##name##_each_reverse(arr_##name v, PFN_arr##name##each cb, void * pUserData) {               \
    col_error_t err = COL_OK;                       \
    /* size_t cannot become negative. So
     * we must call callback for the first
     * element extra */                             \
    for (size_t i = len - 1; i > 0; --i) {          \
        err = cb(v, i, pUserData);                  \
        if (err != COL_OK) {                        \
            return err;                             \
        }                                           \
    }                                               \
    err = cb(v, 0, pUserData);                      \
    if (err != COL_OK) {                            \
        return err;                                 \
    }                                               \
    return err;                                     \
}

#ifdef __cplusplus
};
#endif

#endif //IEW_C_ESSENTIALS_ICEALIGNEDARRAY_H
