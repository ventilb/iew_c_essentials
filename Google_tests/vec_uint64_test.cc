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

#include <cmath>
#include "gtest/gtest.h"
#include "../vec_uint64.h"

TEST(vec_uint64, VecAllocation) {
    vec_uint64 vec = vec_uint64_new();

    EXPECT_NE(nullptr, vec);

    // vector is empty
    EXPECT_EQ(0, vec_uint64_len(vec));
    EXPECT_TRUE(vec_uint64_empty(vec));

    // Implementation detail...newly allocated vectors don't have
    // data member allocated
    EXPECT_EQ(nullptr, vec_uint64_data(vec));

    EXPECT_EQ(COL_OK, vec_uint64_free(vec));
}

TEST(vec_uint64, VecPushBack) {
    uint64_t value = 42;

    vec_uint64 vec = vec_uint64_new();

    // vector is empty...test that underflow error is triggered and the
    // out value did not change
    EXPECT_EQ(COL_ERR_UNDERFLOW, vec_uint64_back(vec, &value));
    EXPECT_EQ(42, value);

    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 1));

    // vector has 1 element
    EXPECT_EQ(1, vec_uint64_len(vec));
    EXPECT_FALSE(vec_uint64_empty(vec));

    // get the last element
    EXPECT_EQ(COL_OK, vec_uint64_back(vec, &value));
    EXPECT_EQ(1, value);
    EXPECT_FALSE(vec_uint64_empty(vec));

    // vector length hasn't changed
    EXPECT_EQ(1, vec_uint64_len(vec));
    EXPECT_FALSE(vec_uint64_empty(vec));

    value = 0;
    EXPECT_EQ(COL_OK, vec_uint64_pop_back(vec, &value));
    EXPECT_EQ(1, value);
    EXPECT_EQ(COL_ERR_UNDERFLOW, vec_uint64_pop_back(vec, nullptr));

    // vector is empty
    EXPECT_EQ(0, vec_uint64_len(vec));
    EXPECT_TRUE(vec_uint64_empty(vec));

    EXPECT_EQ(COL_OK, vec_uint64_free(vec));
}

TEST(vec_uint64, VecClear) {
    vec_uint64 vec = vec_uint64_new();

    EXPECT_EQ(0, vec_uint64_len(vec)); // vector is empty

    vec_uint64_clear(vec);

    EXPECT_EQ(0, vec_uint64_len(vec)); // vector is empty

    EXPECT_EQ(COL_OK, vec_uint64_free(vec));
}

TEST(vec_uint64, VecSetGet) {
    uint64_t value = 42;

    vec_uint64 vec = vec_uint64_new();

    EXPECT_EQ(COL_ERR_ILLEGAL_ARGUMENT, vec_uint64_get(vec, -1, &value));
    EXPECT_EQ(COL_ERR_ILLEGAL_ARGUMENT, vec_uint64_get(vec, 0, &value));
    EXPECT_EQ(COL_ERR_ILLEGAL_ARGUMENT, vec_uint64_get(vec, 1, &value));
    EXPECT_EQ(COL_ERR_ILLEGAL_ARGUMENT, vec_uint64_get(vec, SIZE_MAX, &value));
    EXPECT_EQ(42, value); // Value didn't changed

    EXPECT_EQ(COL_ERR_ILLEGAL_ARGUMENT, vec_uint64_set(vec, -1, 42));
    EXPECT_EQ(COL_ERR_ILLEGAL_ARGUMENT, vec_uint64_set(vec, 0, 42));
    EXPECT_EQ(COL_ERR_ILLEGAL_ARGUMENT, vec_uint64_set(vec, 1, 42));
    EXPECT_EQ(COL_ERR_ILLEGAL_ARGUMENT, vec_uint64_set(vec, SIZE_MAX, 42));

    // Push value '1' onto vector
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 1));
    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 0, &value));
    EXPECT_EQ(1, value);
    EXPECT_EQ(1, vec_uint64_len(vec));

    // Set value at index '0' to '42'
    EXPECT_EQ(COL_OK, vec_uint64_set(vec, 0, 42));
    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 0, &value));
    EXPECT_EQ(42, value);
    EXPECT_EQ(1, vec_uint64_len(vec));

    EXPECT_EQ(COL_OK, vec_uint64_free(vec));
}

TEST(vec_uint64, IteratorTest) {
    uint64_t value = 0;

    vec_uint64 vec = vec_uint64_new();
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 10));
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 100));
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 1000));
    EXPECT_EQ(3, vec_uint64_len(vec));

    iter_uint64 begin = vec_uint64_begin(vec);
    iter_uint64 end = vec_uint64_end(vec);

    EXPECT_EQ(3, end - begin);
    EXPECT_EQ(10, *begin);
    EXPECT_EQ(100, *(begin + 1));
    EXPECT_EQ(1000, *(begin + 2));
    EXPECT_EQ(end, (begin + 3));

    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(pow(10, (i + 1)), *(begin + i));
    }

    int i = 0;
    for (iter_uint64 b = vec_uint64_begin(vec); b < vec_uint64_end(vec); b++) {
        EXPECT_EQ(pow(10, (i + 1)), *b);
        i++;
    }

    for (size_t b = 0; b < vec_uint64_len(vec);) {
        EXPECT_EQ(COL_OK, vec_uint64_get(vec, b, &value));
        if (value == 100) {
            EXPECT_EQ(COL_OK, vec_uint64_erase(vec, b));
        } else {
            b++;
        }
    }
    EXPECT_EQ(2, vec_uint64_len(vec));
    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 0, &value));
    EXPECT_EQ(10, value);
    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 1, &value));
    EXPECT_EQ(1000, value);
    EXPECT_EQ(COL_ERR_ILLEGAL_ARGUMENT, vec_uint64_get(vec, 2, &value));

    EXPECT_EQ(COL_OK, vec_uint64_erase(vec, 0));
    EXPECT_EQ(1, vec_uint64_len(vec));
    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 0, &value));
    EXPECT_EQ(1000, value);

    EXPECT_EQ(COL_OK, vec_uint64_erase(vec, 0));
    EXPECT_EQ(0, vec_uint64_len(vec));

    EXPECT_EQ(COL_OK, vec_uint64_free(vec));
}

TEST(vec_uint64, InsertInMiddle) {
    uint64_t value = 0;

    vec_uint64 vec = vec_uint64_new();
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 10));
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 1000));
    EXPECT_EQ(2, vec_uint64_len(vec));

    EXPECT_EQ(COL_OK, vec_uint64_insert(vec, 1, 100));
    EXPECT_EQ(3, vec_uint64_len(vec));

    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 0, &value));
    EXPECT_EQ(10, value);
    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 1, &value));
    EXPECT_EQ(100, value);
    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 2, &value));
    EXPECT_EQ(1000, value);

    EXPECT_EQ(COL_OK, vec_uint64_free(vec));
}

TEST(vec_uint64, InsertFirst) {
    uint64_t value = 0;

    vec_uint64 vec = vec_uint64_new();
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 100));
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 1000));
    EXPECT_EQ(2, vec_uint64_len(vec));

    EXPECT_EQ(COL_OK, vec_uint64_insert(vec, 0, 10));
    EXPECT_EQ(3, vec_uint64_len(vec));

    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 0, &value));
    EXPECT_EQ(10, value);
    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 1, &value));
    EXPECT_EQ(100, value);
    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 2, &value));
    EXPECT_EQ(1000, value);

    EXPECT_EQ(COL_OK, vec_uint64_free(vec));
}

TEST(vec_uint64, InsertLast) {
    uint64_t value = 0;

    vec_uint64 vec = vec_uint64_new();
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 10));
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 100));
    EXPECT_EQ(2, vec_uint64_len(vec));

    EXPECT_EQ(COL_OK, vec_uint64_insert(vec, 2, 1000));
    EXPECT_EQ(3, vec_uint64_len(vec));

    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 0, &value));
    EXPECT_EQ(10, value);
    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 1, &value));
    EXPECT_EQ(100, value);
    EXPECT_EQ(COL_OK, vec_uint64_get(vec, 2, &value));
    EXPECT_EQ(1000, value);

    EXPECT_EQ(COL_OK, vec_uint64_free(vec));
}

col_error_t vec_uint64_sum(vec_uint64 v, size_t i, void *pData) {
    auto *sum = ((uint64_t *) pData);
    uint64_t x;
    col_error_t err = vec_uint64_get(v, i, &x);

    *sum = *sum + x;

    return err;
}

TEST(vec_uint64, Each) {
    uint64_t sum = 0;

    vec_uint64 vec = vec_uint64_new();
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 10));
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 100));
    EXPECT_EQ(COL_OK, vec_uint64_push_back(vec, 1000));

    EXPECT_EQ(COL_OK, vec_uint64_each(vec, &vec_uint64_sum, &sum));
    EXPECT_EQ(1110, sum);

    EXPECT_EQ(COL_OK, vec_uint64_each_reverse(vec, &vec_uint64_sum, &sum));
    EXPECT_EQ(2220, sum);

    EXPECT_EQ(COL_OK, vec_uint64_free(vec));
}

TEST(vec_uint64, OverflowComputation) {
    const auto GROWTH = (unsigned char) 2;

    // Overflow
    auto len = (unsigned char) 128;
    auto len2 = (unsigned char) (GROWTH * len); // 256
    if (len2 / GROWTH != len) {
    } else {
        FAIL();
    }

    // Overflow
    len = (unsigned char) 127;
    len2 = (unsigned char) (GROWTH * (len + 1)); // 0
    if (len2 / GROWTH != len) {
    } else {
        FAIL();
    }

    // No overflow
    len = (unsigned char) 127;
    len2 = (unsigned char) (GROWTH * len + 1); // 255
    if (len2 / GROWTH != len) {
        FAIL();
    }

    size_t foo = -1;
    size_t bar = SIZE_MAX;
    EXPECT_EQ(bar, foo);

    const auto growth2 = (double) 1.5;
    const auto cur_cap = (size_t) 2;
    auto size = (size_t) ceil((growth2 * ((double) cur_cap + (double) 2)));
    EXPECT_EQ(6, size);
}


int is_aligned(size_t ptr, size_t alignment) {
    return (ptr % alignment) == 0;
}

// https://stackoverflow.com/questions/56619623/why-is-there-no-aligned-realloc-on-most-platforms
TEST(vec_uint64, AlignmentTest) {
    size_t alignment = 8;
    size_t struct_size = 8;
    size_t requested_structs = 1;

    size_t aligned_size = ((requested_structs * struct_size + alignment) & ~(alignment - 1));
    EXPECT_EQ(16, aligned_size);
    EXPECT_EQ(2, aligned_size / struct_size);
    EXPECT_TRUE(is_aligned(aligned_size, alignment));

    alignment = 8;
    struct_size = 3;

    aligned_size = ((requested_structs * struct_size + alignment) & ~(alignment - 1));
    EXPECT_EQ(8, aligned_size);
    EXPECT_EQ(2, aligned_size / struct_size);
    EXPECT_TRUE(is_aligned(aligned_size, alignment));

    alignment = 8;
    struct_size = 7;

    aligned_size = ((requested_structs * struct_size + alignment) & ~(alignment - 1));
    EXPECT_EQ(8, aligned_size);
    EXPECT_EQ(1, aligned_size / struct_size);
    EXPECT_TRUE(is_aligned(aligned_size, alignment));

    alignment = 8;
    struct_size = 2;

    aligned_size = ((requested_structs * struct_size + alignment) & ~(alignment - 1));
    EXPECT_EQ(8, aligned_size);
    EXPECT_EQ(4, aligned_size / struct_size);
    EXPECT_TRUE(is_aligned(aligned_size, alignment));

    alignment = 8;
    struct_size = 1;

    aligned_size = ((requested_structs * struct_size + alignment) & ~(alignment - 1));
    EXPECT_EQ(8, aligned_size);
    EXPECT_EQ(8, aligned_size / struct_size);
    EXPECT_TRUE(is_aligned(aligned_size, alignment));

    alignment = 1;
    struct_size = 3;

    aligned_size = ((requested_structs * struct_size + alignment) & ~(alignment - 1));
    EXPECT_EQ(4, aligned_size);
    EXPECT_EQ(1, aligned_size / struct_size);
    EXPECT_TRUE(is_aligned(aligned_size, alignment));

    alignment = 1;
    struct_size = 2;

    aligned_size = ((requested_structs * struct_size + alignment) & ~(alignment - 1));
    EXPECT_EQ(3, aligned_size);
    EXPECT_EQ(1, aligned_size / struct_size);
    EXPECT_TRUE(is_aligned(aligned_size, alignment));

    EXPECT_TRUE(is_aligned(8, 8));
    EXPECT_TRUE(is_aligned(8, 4));
    EXPECT_TRUE(is_aligned(8, 2));
    EXPECT_TRUE(is_aligned(8, 1));
}

int is_aligned(void *ptr, size_t alignment) {
    return ((uintptr_t) ptr % alignment) == 0;
}

TEST(vec_uint64, ColAlignedAlloc) {
    size_t alignment = 32;

    void *data = col_align_alloc(nullptr, 500, alignment);
    EXPECT_NE(nullptr, data);
    EXPECT_TRUE(is_aligned(data, alignment));

    data = col_align_alloc(data, 16394, alignment);
    EXPECT_NE(nullptr, data);
    EXPECT_TRUE(is_aligned(data, alignment));

    free(data);
}