#include "utils.h"

#if defined(IEW_ENABLE_DEBUG)

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

// https://www.guyrutenberg.com/2008/12/20/expanding-macros-into-string-constants-in-c/
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

void vlog_format(const char *level, const char *message, va_list args) {
    time_t now;
    time(&now);
    char *date = ctime(&now);
    date[strlen(date) - 1] = '\0';
    fprintf(stderr, "%s [%s] ", date, level);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
}

void log_format(const char *level, const char *message, ...) {
    va_list args;
    va_start(args, message);
    time_t now;
    time(&now);
    char *date = ctime(&now);
    date[strlen(date) - 1] = '\0';
    fprintf(stderr, "%s [%s] ", date, level);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void ltrace(const char *message, ...) {
    va_list args;
    va_start(args, message);
    vlog_format("trace", message, args);
    va_end(args);
}

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
        return IEW_FN_ALIGNED_ALLOC(alignment, size);
    }
}