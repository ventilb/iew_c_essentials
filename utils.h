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
