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

#include <string.h>
#include "utils.h"
#include "icelogging.h"
#include "icemalloc.h"

#if defined(IEW_ENABLE_DEBUG)
// https://www.guyrutenberg.com/2008/12/20/expanding-macros-into-string-constants-in-c/
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
#endif

const double VEC_GROWTH = (double) 1.5;

// https://stackoverflow.com/questions/64884745/is-there-a-linux-equivalent-of-aligned-realloc
int col_is_aligned_ptr(void *  ptr, size_t alignment) {
    return ((uintptr_t)ptr % alignment) == 0;
}

void* col_aligned_realloc_pessimistic(void * ptr, size_t new_size, size_t old_size, size_t alignment) {
    void* aligned = IEW_FN_ALIGNED_ALLOC(alignment, new_size);
    if (aligned == NULL)  {
        return NULL;
    }

    memcpy(aligned, ptr, old_size);
    free(ptr);
    return aligned;
}

void* col_align_alloc(void * ptr, size_t size, const size_t alignment) {
    size = ice_align_up(size, alignment);
    ltrace("[col_align_alloc] - size=%d, alignment=%d", size, alignment);
    if (ptr != NULL) {
        void * tmp_ptr = IEW_FN_REALLOC(ptr, size);
        if (tmp_ptr == NULL) {
            IEW_FN_FREE(ptr);
            return NULL;
        }

        if (col_is_aligned_ptr(tmp_ptr, alignment)) {
            return tmp_ptr;
        } else {
            ltrace("[col_align_alloc] - alloc addr %p is not aligned", tmp_ptr);
            return col_aligned_realloc_pessimistic(tmp_ptr, size, size, alignment);
        }
    } else {
        return aligned_alloc(alignment, size);
    }
}