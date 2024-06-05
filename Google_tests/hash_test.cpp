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

#include "gtest/gtest.h"
#include "../hash.h"

TEST(hash_test, fnv_32a) {
    Fnv32_t h;

    h = fnv_32a_byte1(42, FNV1_32A_INIT);
    EXPECT_EQ(0x2f0c9f3d, h);

    // ---

    h = FNV1_32A_INIT;
    // 0x00BC614E
    // On Little Endian => 4E 61 BC 00
    h = fnv_32a_byte1(0x4e, h);
    h = fnv_32a_byte1(0x61, h);
    h = fnv_32a_byte1(0xbc, h);
    h = fnv_32a_byte1(0x00, h);
    EXPECT_EQ(0x2f55268c, h);

    // ---

    h = fnv_32a_uint32_t(12345678, FNV1_32A_INIT); // 0x00BC614E
    EXPECT_EQ(0x2f55268c, h);

    // ---

    h = fnv_32a_uint32_t(287454020, FNV1_32A_INIT); // 0x11223344
    EXPECT_EQ(0xce4211d5, h);

    // ---

    h = fnv_32a_uint32_t(0xFFFFFFFF, FNV1_32A_INIT);
    EXPECT_EQ(0xe3160fb1, h);

    // ---

    // Little-Endian: 88 77 66 55 44 33 22 11
    h = fnv_32a_uint64_t(0x1122334455667788, FNV1_32A_INIT);
    EXPECT_EQ(0xf24b8e95, h);

    // ---

    h = fnv_32a_uint32_t(0x00000000, FNV1_32A_INIT);
    EXPECT_EQ(0x4b95f515, h);

    // ---

    uint64_t i = 0x1122334455667788;
    h = fnv_32a_buf(&i, sizeof (uint64_t), FNV1_32A_INIT);
    EXPECT_EQ(0xf24b8e95, h);

    // ---

    float f = 3.1415f;
    h = fnv_32a_buf(&f, sizeof (float), FNV1_32A_INIT);
    EXPECT_EQ(0xefd4950c, h);

    // ---

    h = fnv_32a_float(3.1415f, FNV1_32A_INIT);
    EXPECT_EQ(0xefd4950c, h);

    // ---

    int i1 = -127;
    h = fnv_32a_buf(&i1, sizeof (int), FNV1_32A_INIT);
    EXPECT_EQ(0x50460f07, h);

    // ---

    h = fnv_32a_int(-127, FNV1_32A_INIT);
    EXPECT_EQ(0x50460f07, h);
}

TEST(hash_test, fnv_64a) {
    Fnv64_t h;

    h = fnv_64a_byte1(42, FNV1A_64_INIT);
    EXPECT_EQ(0xaf63a74c8601927d, h);

    // ---

    h = FNV1A_64_INIT;
    // 0x00BC614E
    // On Little Endian => 4E 61 BC 00
    h = fnv_64a_byte1(0x4e, h);
    h = fnv_64a_byte1(0x61, h);
    h = fnv_64a_byte1(0xbc, h);
    h = fnv_64a_byte1(0x00, h);
    EXPECT_EQ(0xefe9cdc6905a3c8c, h);

    // ---

    h = fnv_64a_uint32_t(12345678, FNV1A_64_INIT); // 0x00BC614E
    EXPECT_EQ(0xefe9cdc6905a3c8c, h);

    // ---

    h = fnv_64a_uint32_t(287454020, FNV1A_64_INIT); // 0x11223344
    EXPECT_EQ(0x3c1b247411d7b2b5, h);

    // ---

    h = fnv_64a_uint32_t(0xFFFFFFFF, FNV1A_64_INIT);
    EXPECT_EQ(0x994f76653e2a3951, h);

    // ---

    // Little-Endian: 88 77 66 55 44 33 22 11
    h = fnv_64a_uint64_t(0x1122334455667788, FNV1A_64_INIT);
    EXPECT_EQ(0x82f950555efebd75, h);

    // ---

    h = fnv_64a_uint32_t(0x00000000, FNV1A_64_INIT);
    EXPECT_EQ(0x4d25767f9dce13f5, h);

    // ---

    uint64_t i = 0x1122334455667788;
    h = fnv_64a_buf(&i, sizeof (uint64_t), FNV1A_64_INIT);
    EXPECT_EQ(0x82f950555efebd75, h);

    // ---

    float f = 3.1415f;
    h = fnv_64a_buf(&f, sizeof (float), FNV1A_64_INIT);
    EXPECT_EQ(0xfb247509d92af7ec, h);

    // ---

    h = fnv_64a_float(3.1415f, FNV1A_64_INIT);
    EXPECT_EQ(0xfb247509d92af7ec, h);

    // ---

    int i1 = -127; // 0xffffff81
    h = fnv_64a_buf(&i1, sizeof (int), FNV1A_64_INIT);
    EXPECT_EQ(0xaabb74a0b07d0de7, h);

    // ---

    h = fnv_64a_int(-127, FNV1A_64_INIT);
    EXPECT_EQ(0xaabb74a0b07d0de7, h);
}