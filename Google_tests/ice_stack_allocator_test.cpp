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
#include "icemalloc.h"

TEST(StackAllocator, Alloc) {
    ice_stack_allocator alloc = ice_stack_malloc_new(64);

    void * p1 = ice_stack_malloc(alloc, 8);
    EXPECT_EQ(alloc->buf, p1);
    EXPECT_EQ(8, alloc->offset);

    void * p2 = ice_stack_zmalloc(alloc, 8);
    EXPECT_EQ(alloc->buf + 8, p2);
    EXPECT_EQ(16, alloc->offset);

    void * p3 = ice_stack_malloc(alloc, 4);
    EXPECT_EQ(alloc->buf + 16, p3);
    EXPECT_EQ(24, alloc->offset);

    void * p4 = ice_stack_zmalloc(alloc, 21);
    EXPECT_EQ(alloc->buf + 24, p4);
    EXPECT_EQ(48, alloc->offset);

    void * p5 = ice_stack_malloc(alloc, 15);
    EXPECT_EQ(alloc->buf + 48, p5);
    EXPECT_EQ(64, alloc->offset);

    void * p6 = ice_stack_zmalloc(alloc, 1);
    EXPECT_EQ(nullptr, p6);
    EXPECT_EQ(64, alloc->offset);

    ice_stack_malloc_free(alloc);
}