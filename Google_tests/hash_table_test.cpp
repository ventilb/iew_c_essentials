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
#include "../ice_hash_table_macros.h"
#include "../icehash.h"
#include "../vec_int.h"
#include "test_data.h"

uint64_t int_hashCode(int k) {
    if (k == 1 || k == 2 || k == 3) {
        // map keys 1, 2, 3 to same hash value => hash conflict
        return (uint64_t) fnv_64a_int(1, FNV1A_64_INIT);
    } else {
        return (uint64_t) fnv_64a_int(k, FNV1A_64_INIT);
    }
}

uint64_t int_bad_hashcode(int k) {
    return 1;
}

uint64_t int_better_hashcode(int k) {
    return fnv_64a_int(k, FNV1A_64_INIT);
}

bool int_comparator(int k1, int k2) {
    return k1 == k2;
}

col_error_t visitVec3(int k, Vec3 v, void * pUserData) {
    auto ints = (vec_int) pUserData;
    return vec_int_push_back(ints, k);
}

makeHashTableApi(Vec3, int, Vec3)
makeHashTableImpl(Vec3, int, Vec3, 0, NULL, int_hashCode, int_comparator)

makeHashTableApi(bad_int, int, int)
makeHashTableImpl(bad_int, int, int, -1, -1, int_bad_hashcode, int_comparator)

makeHashTableApi(better_int, int, int)
makeHashTableImpl(better_int, int, int, -1, -1, int_better_hashcode, int_comparator)

TEST(Hashtable, Vec3Test) {
    ht_Vec3 ht = ht_Vec3_new();

    struct Vec3_T v1 = {.a = 0.1f, .b = 0.2f, .c = 0.3f};
    struct Vec3_T v2 = {.a = 1.1f, .b = 1.2f, .c = 1.3f};
    struct Vec3_T v3 = {.a = 2.1f, .b = 2.2f, .c = 2.3f};

    vec_int visited = vec_int_new();

    EXPECT_EQ(COL_OK, ht_Vec3_each(ht, visitVec3, visited));
    EXPECT_EQ(0, vec_int_len(visited));

    // key 1
    EXPECT_EQ(nullptr, ht_Vec3_put(ht, 1, &v1));

    Vec3 v1_1 = ht_Vec3_get(ht, 1);
    EXPECT_EQ(&v1, v1_1);

    EXPECT_EQ(COL_OK, ht_Vec3_each(ht, visitVec3, visited));
    EXPECT_EQ(1, vec_int_len(visited));

    // key 2
    EXPECT_EQ(nullptr, ht_Vec3_put(ht, 2, &v2));

    v1_1 = ht_Vec3_get(ht, 1);
    Vec3 v2_1 = ht_Vec3_get(ht, 2);
    EXPECT_EQ(&v1, v1_1);
    EXPECT_EQ(&v2, v2_1);

    EXPECT_EQ(COL_OK, ht_Vec3_each(ht, visitVec3, visited));
    EXPECT_EQ(3, vec_int_len(visited));

    EXPECT_EQ(&v1, ht_Vec3_put(ht, 1, nullptr));

    v1_1 = ht_Vec3_get(ht, 1);
    v2_1 = ht_Vec3_get(ht, 2);
    EXPECT_EQ(nullptr, v1_1);
    EXPECT_EQ(&v2, v2_1);

    EXPECT_EQ(COL_OK, ht_Vec3_each(ht, visitVec3, visited));
    EXPECT_EQ(4, vec_int_len(visited));

    EXPECT_EQ(nullptr, ht_Vec3_put(ht, 1, &v1));

    EXPECT_EQ(COL_OK, ht_Vec3_each(ht, visitVec3, visited));
    EXPECT_EQ(6, vec_int_len(visited));

    v1_1 = ht_Vec3_get(ht, 1);
    v2_1 = ht_Vec3_get(ht, 2);
    EXPECT_EQ(&v1, v1_1);
    EXPECT_EQ(&v2, v2_1);

    EXPECT_EQ(nullptr, ht_Vec3_put(ht, 3, &v3));

    v1_1 = ht_Vec3_get(ht, 1);
    v2_1 = ht_Vec3_get(ht, 2);
    Vec3 v3_1 = ht_Vec3_get(ht, 3);
    EXPECT_EQ(&v1, v1_1);
    EXPECT_EQ(&v2, v2_1);
    EXPECT_EQ(&v3, v3_1);
    EXPECT_EQ(3, ht_Vec3_len(ht));

    EXPECT_EQ(COL_OK, ht_Vec3_each(ht, visitVec3, visited));
    EXPECT_EQ(9, vec_int_len(visited));

    EXPECT_EQ(&v2, ht_Vec3_put(ht, 2, nullptr));

    v1_1 = ht_Vec3_get(ht, 1);
    v2_1 = ht_Vec3_get(ht, 2);
    v3_1 = ht_Vec3_get(ht, 3);
    EXPECT_EQ(&v1, v1_1);
    EXPECT_EQ(nullptr, v2_1);
    EXPECT_EQ(&v3, v3_1);
    EXPECT_EQ(2, ht_Vec3_len(ht));

    EXPECT_EQ(COL_OK, ht_Vec3_each(ht, visitVec3, visited));
    EXPECT_EQ(11, vec_int_len(visited));

    EXPECT_EQ(&v1, ht_Vec3_put(ht, 1, nullptr));
    EXPECT_EQ(nullptr, ht_Vec3_put(ht, 2, nullptr));
    EXPECT_EQ(&v3, ht_Vec3_put(ht, 3, nullptr));
    EXPECT_EQ(0, ht_Vec3_len(ht));

    EXPECT_EQ(COL_OK, ht_Vec3_each(ht, visitVec3, visited));
    EXPECT_EQ(11, vec_int_len(visited));

    EXPECT_EQ(nullptr, ht_Vec3_put(ht, 1, nullptr));
    EXPECT_EQ(nullptr, ht_Vec3_put(ht, 2, nullptr));
    EXPECT_EQ(nullptr, ht_Vec3_put(ht, 3, nullptr));
    EXPECT_EQ(0, ht_Vec3_len(ht));

    EXPECT_EQ(COL_OK, ht_Vec3_each(ht, visitVec3, visited));
    EXPECT_EQ(11, vec_int_len(visited));

    EXPECT_EQ(nullptr, ht_Vec3_get(ht, 4));

    ht_Vec3_free(ht);
    vec_int_free(visited);
}

TEST(Hashtable, BadHashTest) {
    ht_bad_int ht = ht_bad_int_new();

    const int iters = 100;

    int cnt = 0;
    for (int i = 0; i < iters; ++i) {
        fprintf(stderr, "I: %d\n", i);
        EXPECT_EQ(-1, ht_bad_int_put(ht, i, i));
        cnt++;
    }
    EXPECT_EQ(iters, cnt);
    EXPECT_EQ(iters, ht_bad_int_len(ht));

    for (int i = 0; i < iters; ++i) {
        EXPECT_EQ(i, ht_bad_int_get(ht, i));
    }
    EXPECT_EQ(iters, ht_bad_int_len(ht));

    for (int i = 0; i < iters; ++i) {
        EXPECT_EQ(i, ht_bad_int_erase(ht, i));
    }
    EXPECT_EQ(0, ht_bad_int_len(ht));

    ht_bad_int_free(ht);
}


TEST(Hashtable, BetterHashTest) {
    ht_better_int ht = ht_better_int_new();

    const int iters = 100;

    for (int i = 0; i < iters; ++i) {
        EXPECT_EQ(-1, ht_better_int_put(ht, i, i));
    }
    EXPECT_EQ(iters, ht_better_int_len(ht));

    for (int i = 0; i < iters; ++i) {
        EXPECT_EQ(i, ht_better_int_get(ht, i));
    }
    EXPECT_EQ(iters, ht_better_int_len(ht));

    for (int i = 0; i < iters; ++i) {
        EXPECT_EQ(i, ht_better_int_erase(ht, i));
    }
    EXPECT_EQ(0, ht_better_int_len(ht));

    ht_better_int_free(ht);
}