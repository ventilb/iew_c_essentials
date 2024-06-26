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

#include "icemalloc.h"
#include "icelogging.h"
#include "col_error.h"
#include "string.h"

typedef u_int16_t offset_t;
#define PTR_OFFSET_SZ sizeof(offset_t)

void * ice_aligned_malloc(size_t align, size_t size) {
    ltrace("[ice_aligned_malloc] - align=%ld, size=%ld", align, size);
    void * ptr = NULL;

    IVK_ASSERT((align & (align - 1)) == 0, "align must be power of 2");

    if (align > 0 && size > 0) {
        const size_t hdr_size = PTR_OFFSET_SZ + (align - 1);
        ltrace("[ice_aligned_malloc] - hdr_size=%ld, alloc size=%ld", hdr_size, size+hdr_size);

        const void * p = IEW_FN_MALLOC(size + hdr_size);
        ltrace("[ice_aligned_malloc] - pointer malloc=%ld", p);

        if (p != NULL) {
            ptr = (void *) ice_align_up(((uintptr_t ) p + PTR_OFFSET_SZ), align);
            ltrace("[ice_aligned_malloc] - pointer aligned=%ld", ptr);

            *((offset_t *) ptr - 1) = (offset_t)((uintptr_t ) ptr - (uintptr_t ) p);
            ltrace("[ice_aligned_malloc] - offset=%ld", *((offset_t *) ptr - 1));
        }
    }
    return ptr;
}

void * ice_aligned_zmalloc(size_t align, size_t size) {
    ltrace("[ice_aligned_zmalloc] - align=%ld, size=%ld", align, size);
    void * ptr = NULL;

    IVK_ASSERT((align & (align - 1)) == 0, "align must be power of 2");

    if (align > 0 && size > 0) {
        const size_t hdr_size = PTR_OFFSET_SZ + (align - 1);
        ltrace("[ice_aligned_zmalloc] - hdr_size=%ld, alloc size=%ld", hdr_size, size+hdr_size);

        const void * p = IEW_FN_MALLOC(size + hdr_size);
        ltrace("[ice_aligned_zmalloc] - pointer malloc=%ld", p);

        if (p != NULL) {
            // This is unsafe because compiler might optimize away
            // this memset call. Haven't found how to use memset_s yet.
            memset((void *) p, 0, size + hdr_size);
            ptr = (void *) ice_align_up(((uintptr_t ) p + PTR_OFFSET_SZ), align);
            ltrace("[ice_aligned_malloc] - pointer aligned=%ld", ptr);

            *((offset_t *) ptr - 1) = (offset_t)((uintptr_t ) ptr - (uintptr_t ) p);
            ltrace("[ice_aligned_malloc] - offset=%ld", *((offset_t *) ptr - 1));
        }
    }
    return ptr;
}

void ice_aligned_free(void * ptr) {
    if (ptr == NULL) {
        return;
    }
    offset_t offset = *((offset_t *) ptr - 1);
    void * p = (void *)((uint8_t *) ptr - offset);
    IEW_FN_FREE(p);
}

void* ice_stack_malloc(ice_stack_allocator allocator, size_t size) {
    if (size == 0) {
        return NULL;
    }
    size = ice_align_up(size, PTR_ALIGN);

    if (size > (allocator->size - allocator->offset)) {
        return NULL;
    }

    void *ptr = (allocator->buf + allocator->offset);

    allocator->offset += size;

    return ptr;
}

void* ice_stack_zmalloc(ice_stack_allocator allocator, size_t size) {
    if (size == 0) {
        return NULL;
    }
    size = ice_align_up(size, PTR_ALIGN);

    if (size > (allocator->size - allocator->offset)) {
        return NULL;
    }

    void *ptr = (allocator->buf + allocator->offset);
    memset((void *) ptr, 0, size);

    allocator->offset += size;

    return ptr;
}
