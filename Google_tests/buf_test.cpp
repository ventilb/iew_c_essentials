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
#include "../buf_macros.h"
#include "test_data.h"

makeBufOfTypeApi(Vec3, struct Vec3_T)
makeBufOfTypeImpl(Vec3, struct Vec3_T);

TEST(buf_test, create_buf) {
    buf_Vec3 buf = buf_Vec3_new(CACHE_LINE_SIZE);
    EXPECT_EQ(CACHE_LINE_SIZE, buf->alignedSize);

    EXPECT_EQ(COL_OK, buf_Vec3_reserve(buf, 3));
    EXPECT_NE(nullptr, buf->data);
    EXPECT_EQ(5, buf->cap); // GROWTH has effect too

    // Buffer has memory but limit not set yet
    EXPECT_EQ(COL_ERR_OVERFLOW, buf_Vec3_get(buf, 0, nullptr));
    EXPECT_EQ(COL_ERR_UNDERFLOW, buf_Vec3_back(buf, nullptr));

    // Limit clamped to capacity
    buf_Vec3_set_lim(buf, 10);
    EXPECT_EQ(5, buf->lim);
    EXPECT_EQ(5, buf->cap);

    buf_Vec3_set_lim(buf, 2);
    EXPECT_EQ(2, buf->lim);
    EXPECT_EQ(5, buf->cap);

    Vec3 v1 = nullptr;
    EXPECT_EQ(COL_OK, buf_Vec3_get(buf, 0, &v1));
    EXPECT_EQ(buf->data, (char*) v1);
    EXPECT_EQ(COL_OK, buf_Vec3_memset(buf, 0, 0));

    Vec3 v2 = nullptr;
    EXPECT_EQ(COL_OK, buf_Vec3_get(buf, 1, &v2));
    EXPECT_EQ(buf->data + CACHE_LINE_SIZE, (char*) v2);
    EXPECT_EQ(COL_OK, buf_Vec3_memset(buf, 1, 1));

    EXPECT_EQ(0.0f, v1->a);
    EXPECT_EQ(0.0f, v1->b);
    EXPECT_EQ(0.0f, v1->c);
    EXPECT_NE(0.0f, v2->a);
    EXPECT_NE(0.0f, v2->b);
    EXPECT_NE(0.0f, v2->c);

    // Read beyond limit => Overflow error
    EXPECT_EQ(COL_ERR_OVERFLOW, buf_Vec3_get(buf, 2, nullptr));

    Vec3 v3 = nullptr;
    EXPECT_EQ(COL_OK, buf_Vec3_back(buf, &v3));
    EXPECT_EQ(v2, v3);

    buf_Vec3_free(buf);
}

col_error_t find_vec(buf_Vec3 v, size_t i, bool *pMatch, void * pUserData) {
    float f = *((float*) pUserData);
    Vec3 vec = nullptr;
    buf_Vec3_get(v, i, &vec);
    *pMatch = (vec->a == f && vec->b == f && vec->c == f);
    return COL_OK;
}

TEST(buf_test, search) {
    buf_Vec3 buf = buf_Vec3_new(CACHE_LINE_SIZE);


    EXPECT_EQ(COL_OK, buf_Vec3_reserve(buf, 3));
    buf_Vec3_set_lim(buf, 10);

    Vec3 v = nullptr;
    EXPECT_EQ(COL_OK, buf_Vec3_get(buf, 0, &v));
    v->a = 1.0f;
    v->b = 1.0f;
    v->c = 1.0f;

    EXPECT_EQ(COL_OK, buf_Vec3_get(buf, 1, &v));
    v->a = 2.0f;
    v->b = 2.0f;
    v->c = 2.0f;

    EXPECT_EQ(COL_OK, buf_Vec3_get(buf, 2, &v));
    v->a = 3.0f;
    v->b = 3.0f;
    v->c = 3.0f;

    float lookup = 2.0f;
    size_t index = 0;
    EXPECT_EQ(COL_OK, buf_Vec3_search(buf, find_vec, &index, &lookup));
    EXPECT_EQ(1, index);

    lookup = 1.0f;
    EXPECT_EQ(COL_OK, buf_Vec3_search(buf, find_vec, &index, &lookup));
    EXPECT_EQ(0, index);

    lookup = 3.0f;
    EXPECT_EQ(COL_OK, buf_Vec3_search(buf, find_vec, &index, &lookup));
    EXPECT_EQ(2, index);

    // Not found
    lookup = 4.0f;
    EXPECT_EQ(COL_OK, buf_Vec3_search(buf, find_vec, &index, &lookup));
    EXPECT_EQ(buf_Vec3_lim(buf), index);

    buf_Vec3_free(buf);
}

TEST(buf_test, emplace_back) {
    buf_Vec3 buf = buf_Vec3_new(1);

    Vec3 v1 = nullptr;
    EXPECT_EQ(COL_OK, buf_Vec3_emplace_back(buf, &v1));
    EXPECT_EQ(buf->data, (char*) v1);
    EXPECT_EQ(1, buf_Vec3_lim(buf));

    Vec3 v2 = nullptr;
    EXPECT_EQ(COL_OK, buf_Vec3_emplace_back(buf, &v2));
    EXPECT_EQ(buf->data + 12, (char*) v2);
    EXPECT_EQ(2, buf_Vec3_lim(buf));

    Vec3 v3 = nullptr;
    EXPECT_EQ(COL_OK, buf_Vec3_emplace_back(buf, &v3));
    EXPECT_EQ(buf->data + 24, (char*) v3);
    EXPECT_EQ(3, buf_Vec3_lim(buf));

    buf_Vec3_free(buf);
}