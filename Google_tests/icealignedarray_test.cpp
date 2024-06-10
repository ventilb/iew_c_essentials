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
#include "../icealignedarray.h"

struct Vec3_T {
    float a, b, c;
};
typedef struct Vec3_T * Vec3;

makeArrayOfTypeApi(ArrVec3, struct Vec3_T)
makeArrayOfTypeImpl(ArrVec3, struct Vec3_T, 16, 3)

col_error_t Vec3Init(arr_ArrVec3 a, size_t i, void * pUserData) {
    Vec3 v = nullptr;
    col_error_t err = arr_ArrVec3_get(a, i, &v);
    if (err != COL_OK) {
        return err;
    }
    v->a = (float) (i * 3) + 1;
    v->b = (float) (i * 3) + 2;
    v->c = (float) (i * 3) + 3;
    return COL_OK;
}


TEST(aligned_array_test, create_array) {
    EXPECT_EQ(12, sizeof(Vec3_T));

    arr_ArrVec3 a = arr_ArrVec3_new();
    EXPECT_EQ(3, arr_ArrVec3_len(a));

    //EXPECT_EQ(COL_OK, arr_ArrVec3_each(a, &Vec3Init, nullptr));
    EXPECT_EQ(COL_OK, arr_ArrVec3_each_reverse(a, &Vec3Init, nullptr));

    Vec3 v1 = nullptr;
    EXPECT_EQ(COL_OK, arr_ArrVec3_get(a, 0, &v1));
    EXPECT_EQ((uintptr_t) a, (uintptr_t) v1);
    EXPECT_EQ(1.0f, v1->a);
    EXPECT_EQ(2.0f, v1->b);
    EXPECT_EQ(3.0f, v1->c);

    Vec3 v2 = nullptr;
    EXPECT_EQ(COL_OK, arr_ArrVec3_get(a, 1, &v2));
    EXPECT_EQ((uintptr_t) a + 16, (uintptr_t) v2);
    EXPECT_EQ(4.0f, v2->a);
    EXPECT_EQ(5.0f, v2->b);
    EXPECT_EQ(6.0f, v2->c);

    Vec3 v3 = nullptr;
    EXPECT_EQ(COL_OK, arr_ArrVec3_get(a, 2, &v3));
    EXPECT_EQ((uintptr_t) a + 32, (uintptr_t) v3);
    EXPECT_EQ(7.0f, v3->a);
    EXPECT_EQ(8.0f, v3->b);
    EXPECT_EQ(9.0f, v3->c);

    Vec3 v4 = nullptr;
    EXPECT_EQ(COL_OK, arr_ArrVec3_back(a, &v4));
    EXPECT_EQ((uintptr_t) v3, (uintptr_t) v4);

    EXPECT_EQ(COL_OK, arr_ArrVec3_free(a));
}