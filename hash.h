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

#ifndef IEW_C_ESSENTIALS_HASH_H
#define IEW_C_ESSENTIALS_HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "fnv.h"
#include "col_error.h"

// copied from fnv_hash: hash_32a.c and hash_64a.c because
// these constants are not defined in a lib header and not
// accessible to us
#define FNV_32_PRIME ((Fnv32_t)0x01000193)
#define FNV_64_PRIME ((Fnv64_t)0x100000001b3ULL)

// --- FNV32a functions

extern inline Fnv32_t fnv_32a_byte1(unsigned char byte, Fnv32_t h) {
    return (byte ^ h) * FNV_32_PRIME;
}

extern inline Fnv32_t fnv_32a_byte4(unsigned char const b[4], Fnv32_t h) {
    h = ((unsigned char) (b[0]) ^ h) * FNV_32_PRIME;
    h = ((unsigned char) (b[1]) ^ h) * FNV_32_PRIME;
    h = ((unsigned char) (b[2]) ^ h) * FNV_32_PRIME;
    return ((unsigned char) (b[3]) ^ h) * FNV_32_PRIME;
}

extern inline Fnv32_t fnv_32a_byte8(unsigned char const b[8], Fnv32_t h) {
    h = ((unsigned char) (b[0]) ^ h) * FNV_32_PRIME;
    h = ((unsigned char) (b[1]) ^ h) * FNV_32_PRIME;
    h = ((unsigned char) (b[2]) ^ h) * FNV_32_PRIME;
    h = ((unsigned char) (b[3]) ^ h) * FNV_32_PRIME;
    h = ((unsigned char) (b[4]) ^ h) * FNV_32_PRIME;
    h = ((unsigned char) (b[5]) ^ h) * FNV_32_PRIME;
    h = ((unsigned char) (b[6]) ^ h) * FNV_32_PRIME;
    return ((unsigned char) (b[7]) ^ h) * FNV_32_PRIME;
}

extern inline Fnv32_t fnv_32a_int(int i, Fnv32_t h) {
    COL_ASSERT(sizeof(int) == 4, "int has size 4byte")

    return fnv_32a_byte4((unsigned char *) &i, h);
}

extern inline Fnv32_t fnv_32a_uint32_t(uint32_t i, Fnv32_t h) {
    COL_ASSERT(sizeof(uint32_t) == 4, "uint32_t has size 4byte")

    return fnv_32a_byte4((unsigned char *) &i, h);
}

extern inline Fnv32_t fnv_32a_uint64_t(uint64_t i, Fnv32_t h) {
    COL_ASSERT(sizeof(uint64_t) == 8, "uint64_t has size 8byte")

    return fnv_32a_byte8((unsigned char *) &i, h);
}

extern inline Fnv32_t fnv_32a_float(float f, Fnv32_t h) {
    COL_ASSERT(sizeof(float) == 4, "float has size 4byte")

    return fnv_32a_byte4((unsigned char const *) &f, h);
}

// --- FNV64a functions

extern inline Fnv64_t fnv_64a_byte1(unsigned char byte, Fnv64_t h) {
    return (byte ^ h) * FNV_64_PRIME;
}

extern inline Fnv64_t fnv_64a_byte4(unsigned char const b[4], Fnv64_t h) {
    h = ((unsigned char) (b[0]) ^ h) * FNV_64_PRIME;
    h = ((unsigned char) (b[1]) ^ h) * FNV_64_PRIME;
    h = ((unsigned char) (b[2]) ^ h) * FNV_64_PRIME;
    return ((unsigned char) (b[3]) ^ h) * FNV_64_PRIME;
}

extern inline Fnv64_t fnv_64a_byte8(unsigned char const b[8], Fnv64_t h) {
    h = ((unsigned char) (b[0]) ^ h) * FNV_64_PRIME;
    h = ((unsigned char) (b[1]) ^ h) * FNV_64_PRIME;
    h = ((unsigned char) (b[2]) ^ h) * FNV_64_PRIME;
    h = ((unsigned char) (b[3]) ^ h) * FNV_64_PRIME;
    h = ((unsigned char) (b[4]) ^ h) * FNV_64_PRIME;
    h = ((unsigned char) (b[5]) ^ h) * FNV_64_PRIME;
    h = ((unsigned char) (b[6]) ^ h) * FNV_64_PRIME;
    return ((unsigned char) (b[7]) ^ h) * FNV_64_PRIME;
}

extern inline Fnv64_t fnv_64a_int(int i, Fnv64_t h) {
    COL_ASSERT(sizeof(int) == 4, "int has size 4byte")

    return fnv_64a_byte4((unsigned char *) &i, h);
}

extern inline Fnv64_t fnv_64a_uint32_t(uint32_t i, Fnv64_t h) {
    COL_ASSERT(sizeof(uint32_t) == 4, "uint32_t has size 4byte")

    return fnv_64a_byte4((unsigned char *) &i, h);
}

extern inline Fnv64_t fnv_64a_uint64_t(uint64_t i, Fnv64_t h) {
    COL_ASSERT(sizeof(uint64_t) == 8, "uint64_t has size 8byte")

    return fnv_64a_byte8((unsigned char *) &i, h);
}

extern inline Fnv64_t fnv_64a_float(float f, Fnv64_t h) {
    COL_ASSERT(sizeof(float) == 4, "float has size 4byte")

    return fnv_64a_byte4((unsigned char const *) &f, h);
}

#ifdef __cplusplus
}
#endif

#endif //IEW_C_ESSENTIALS_HASH_H
