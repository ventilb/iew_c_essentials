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

#ifndef IEW_C_ESSENTIALS_ICE_BITS_H
#define IEW_C_ESSENTIALS_ICE_BITS_H

#include <stdint.h>

#ifdef __GNUC__

#define UINT32_BITS sizeof(uint32_t) * 8

#define GetNumberOfLeadingZeros(x) ((x) == 0 ? UINT32_BITS : __builtin_clz(x))

#elif defined(_MSC_VER)
#else
#error "Unsupported platform for bits implementation!"
#endif

static inline uint32_t iceFloorLog2Uint32(uint32_t v) {
    return 31 - GetNumberOfLeadingZeros(v);
}

// https://codeforwin.org/2016/01/c-program-to-count-trailing-zeros-in-binary-number-using-bitwise-operator.html
static inline uint32_t GetNumberOfTrailingZerosUint32(uint32_t word) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < UINT32_BITS; i ++) {
        /* If set bit is found the terminate from loop*/
        if ((word >> i) & 1) {
            return count;
        }
        count ++;
    }
    return count;
}

/**
 * Macro used to visit each set bit in word. Sets the bit position
 * to variable bit_var and executes the code in body.
 */
#define ForEachBitPos_uint32(word, bit_var, body) \
{ \
    uint32_t t_6787549783, w_09284374; \
    w_09284374 = word; \
    while (w_09284374 != 0) { \
        t_6787549783 = w_09284374 & -w_09284374; \
        bit_var = GetNumberOfTrailingZerosUint32(w_09284374); \
        body \
        w_09284374 = (w_09284374^t_6787549783); \
    } \
}


#endif //IEW_C_ESSENTIALS_ICE_BITS_H
