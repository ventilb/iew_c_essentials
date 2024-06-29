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

#ifndef IEW_C_ESSENTIALS_ICEMALLOC_H
#define IEW_C_ESSENTIALS_ICEMALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdalign.h>
#include <stddef.h>

#define CACHE_LINE_SIZE 64
#define PTR_ALIGN alignof(void *)

extern const double VEC_GROWTH;

// https://embeddedartistry.com/blog/2017/02/22/generating-aligned-memory/

typedef u_int16_t offset_t;
#define PTR_OFFSET_SZ sizeof(offset_t)

/**
 * Used to align an integer value (int, uint_t, uintptr_t,...) to it's upper
 * alignment align.
 */
#ifndef ice_align_up
#define ice_align_up(num, align) \
    (((num) + ((align) - 1)) & ~((align) - 1))
#endif

#define ice_offset_of(memblock) *((offset_t *) (memblock) - 1)

#define ice_is_aligned(memblock, align) ((uintptr_t) (memblock) % (align)) == 0

void * ice_aligned_realloc(void * memblock, size_t align, size_t old_size, size_t new_size);

void *ice_aligned_malloc(size_t align, size_t size);

void *ice_aligned_zmalloc(size_t align, size_t size);

void ice_aligned_free(void *ptr);

#define ice_malloc_cache_aligned(s) ice_aligned_malloc(CACHE_LINE_SIZE, s)
#define ice_zmalloc_cache_aligned(s) ice_aligned_zmalloc(CACHE_LINE_SIZE, s)
#define ice_malloc_ptr_aligned(s) ice_aligned_malloc(PTR_ALIGN, s)
#define ice_zmalloc_ptr_aligned(s) ice_aligned_zmalloc(PTR_ALIGN, s)

typedef struct ice_stack_allocator_t {
    size_t offset;
    size_t size;
    char *buf;
} *ice_stack_allocator;

static inline ice_stack_allocator ice_stack_malloc_new(size_t size) {
    size = ice_align_up(size, PTR_ALIGN);
    struct ice_stack_allocator_t *allocator =
            (struct ice_stack_allocator_t *) ice_malloc_ptr_aligned(sizeof(struct ice_stack_allocator_t) + size);
    if (allocator != NULL) {
        allocator->buf = (char*) allocator + sizeof(size_t)/*offset*/ + sizeof(size_t)/*size*/;
        allocator->offset = 0;
        allocator->size = size;
    }
    return allocator;
}

void *ice_stack_malloc(ice_stack_allocator allocator, size_t size);

void *ice_stack_zmalloc(ice_stack_allocator allocator, size_t size);

static inline void ice_stack_malloc_free(ice_stack_allocator allocator) {
    if (allocator != NULL) {
        ice_aligned_free(allocator);
    }
}

#ifdef __cplusplus
}
#endif

#endif //IEW_C_ESSENTIALS_ICEMALLOC_H
