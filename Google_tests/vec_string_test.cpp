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
#include "../vec_string.h"
#include "../icestring.h"

TEST(vec_string, VecStringTest) {
    vec_string vec = vec_string_new();

    EXPECT_EQ(COL_OK, vec_string_push_back(vec, "H"));
    EXPECT_EQ(COL_OK, vec_string_push_back(vec, "e"));
    EXPECT_EQ(COL_OK, vec_string_push_back(vec, "l"));
    EXPECT_EQ(COL_OK, vec_string_push_back(vec, "l"));
    EXPECT_EQ(COL_OK, vec_string_push_back(vec, "o"));
    EXPECT_EQ(COL_OK, vec_string_push_back(vec, " "));
    EXPECT_EQ(COL_OK, vec_string_push_back(vec, "W"));
    EXPECT_EQ(COL_OK, vec_string_push_back(vec, "o"));
    EXPECT_EQ(COL_OK, vec_string_push_back(vec, "r"));
    EXPECT_EQ(COL_OK, vec_string_push_back(vec, "l"));
    EXPECT_EQ(COL_OK, vec_string_push_back(vec, "d"));
    EXPECT_EQ(COL_OK, vec_string_push_back(vec, "\0"));

    // 12 strings in the vector
    EXPECT_EQ(12, vec_string_len(vec));
    EXPECT_EQ(1, strlen(vec_string_data(vec)[0]));
    EXPECT_STRCASEEQ("H", vec_string_data(vec)[0]);
    EXPECT_EQ(1, strlen(vec_string_data(vec)[1]));
    EXPECT_STRCASEEQ("e", vec_string_data(vec)[1]);

    EXPECT_EQ(1, strlen(vec_string_data(vec)[5]));
    EXPECT_STRCASEEQ(" ", vec_string_data(vec)[5]);

    char * helloWorld = str_of("Hello World");
    EXPECT_EQ(COL_OK, vec_string_set(vec, 5, helloWorld));

    EXPECT_EQ(11, strlen(vec_string_data(vec)[5]));
    EXPECT_STRCASEEQ("Hello World", vec_string_data(vec)[5]);

    EXPECT_EQ(COL_OK, vec_string_erase(vec, 11));
    EXPECT_EQ(11, vec_string_len(vec));
}
