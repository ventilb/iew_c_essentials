#ifndef VEC_CHAR_H
#define VEC_CHAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "macros.h"

makePrimitiveVecOfTypeApi(char)
makeVecApi(char)

char* vec_char_string(vec_char v);

#ifdef __cplusplus
};
#endif

#endif // VEC_CHAR_H