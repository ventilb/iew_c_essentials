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
#include "../icemalloc.h"

TEST(ice_tests, aligned_malloc_test) {
    void * ptr = ice_aligned_malloc(16, 18);
    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(0, ((uintptr_t) ptr) % 16);
    EXPECT_EQ(16, ice_offset_of(ptr));
    ice_aligned_free(ptr);

    ptr = ice_aligned_malloc(4, 18);
    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(0, ((uintptr_t) ptr) % 4);
    EXPECT_EQ(4, ice_offset_of(ptr));
    ice_aligned_free(ptr);

    ptr = ice_aligned_malloc(1, 18);
    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(0, ((uintptr_t) ptr) % 1);
    EXPECT_EQ(2, ice_offset_of(ptr));
    ice_aligned_free(ptr);
}


TEST(ice_tests, aligned_realloc_test) {
    char * ptr = (char *) ice_aligned_realloc(nullptr, 16, 0, 5);
    EXPECT_NE(nullptr, ptr);
    EXPECT_EQ(0, ((uintptr_t) ptr) % 16);
    EXPECT_EQ(16, ice_offset_of(ptr));
    ptr[0] = 'H';
    ptr[1] = 'a';
    ptr[2] = 'l';
    ptr[3] = 'l';
    ptr[4] = '\0';
    EXPECT_STREQ("Hall", ptr);

    ice_aligned_malloc(16, 23);

    char * ptr1 = (char *) ice_aligned_realloc(ptr, 4, 5, 11);
    EXPECT_NE(nullptr, ptr1);
    EXPECT_EQ(0, ((uintptr_t) ptr1) % 4);
    EXPECT_EQ(4, ice_offset_of(ptr1));
    EXPECT_STREQ("Hall", ptr1);

    ptr1[4] = 'o';
    ptr1[5] = ' ';
    ptr1[6] = 'W';
    ptr1[7] = 'e';
    ptr1[8] = 'l';
    ptr1[9] = 't';
    ptr1[10] = '\0';
    EXPECT_STREQ("Hallo Welt", ptr1);

    ice_aligned_free(ptr1);
}