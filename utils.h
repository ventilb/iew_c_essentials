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
#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

// https://stackoverflow.com/questions/5891221/variadic-macros-with-zero-arguments
#define VEC_VA_ARGS(...) , ##__VA_ARGS__

#define VEC_TRUE (1)
#define VEC_FALSE (0)

#define VEC_ASSERT(x)                                            \
	do                                                           \
	{                                                            \
		if (!(x))                                            \
		{                                                        \
			printf("Vector error at %s:%d.\n", __FILE__, __LINE__); \
			abort();                                        \
		}                                                        \
	} while (0);

#if defined(IEW_ENABLE_DEBUG)
void ltrace(const char* message, ...);
#else
#define ltrace(_m, ...)
#endif

extern const double VEC_GROWTH;

void* col_align_alloc(void * ptr, size_t size, const size_t alignment);

#ifdef __cplusplus
};
#endif

#endif // UTILS_H
