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
#include "../icestring.h"

TEST(string_builder, StrOfEmpty) {
    char * str = str_of_empty();

    EXPECT_EQ(0, str_len(str));
    EXPECT_EQ(0, str_size(str));
    EXPECT_STREQ("", str);

    str_free(str);
}

TEST(string_builder, StrOf) {
    char * str = str_of("a猫🍌");

    EXPECT_EQ(3, str_len(str));
    EXPECT_EQ(8, str_size(str)); // Doesn't count the '\0' byte
    EXPECT_STREQ("a猫🍌", str);

    str_free(str);

    str = str_of("");

    EXPECT_EQ(0, str_len(str));
    EXPECT_EQ(0, str_size(str));
    EXPECT_STREQ("", str);

    str_free(str);

    str = str_of(nullptr);

    EXPECT_EQ(0, str_len(str));
    EXPECT_EQ(0, str_size(str));
    EXPECT_STREQ("", str);

    str_free(str);
}

TEST(string_builder, StrAppend) {
    char * str = str_of(nullptr);

    str = str_append(str, "a");
    EXPECT_EQ(1, str_len(str));
    EXPECT_EQ(1, str_size(str));
    EXPECT_STREQ("a", str);

    str = str_append(str, "猫");
    EXPECT_EQ(2, str_len(str));
    EXPECT_EQ(4, str_size(str));
    EXPECT_STREQ("a猫", str);

    str = str_append(str, "🍌c");
    EXPECT_EQ(4, str_len(str));
    EXPECT_EQ(9, str_size(str));
    EXPECT_STREQ("a猫🍌c", str);

    str_free(str);
}

TEST(string_builder, StrIndexRune) {
    char * str = str_of("a猫🍌c");

    EXPECT_EQ(0, str_index_rune(str, u'a'));
    EXPECT_EQ(1, str_index_rune(str, u'猫'));
    EXPECT_EQ(2, str_index_rune(str, U'🍌'));
    EXPECT_EQ(3, str_index_rune(str, u'c'));
    EXPECT_EQ(-1, str_index_rune(str, u'b'));
    EXPECT_EQ(0, str_index_rune(str, '\0'));
    EXPECT_EQ(0, str_index_rune("\0", '\0'));
    EXPECT_EQ(-1, str_index_rune("\0", 'a'));

    str_free(str);
}

TEST(string_builder, StrIndexString) {
    char * str = str_of("a猫🍌c");

    EXPECT_EQ(0, str_index_string(str, "a"));
    EXPECT_EQ(0, str_index_string(str, "a猫"));
    EXPECT_EQ(0, str_index_string(str, "a猫🍌"));
    EXPECT_EQ(0, str_index_string(str, "a猫🍌c"));

    EXPECT_EQ(1, str_index_string(str, "猫"));
    EXPECT_EQ(1, str_index_string(str, "猫🍌"));
    EXPECT_EQ(1, str_index_string(str, "猫🍌c"));

    EXPECT_EQ(2, str_index_string(str, "🍌"));
    EXPECT_EQ(2, str_index_string(str, "🍌c"));

    EXPECT_EQ(3, str_index_string(str, "c"));
    EXPECT_EQ(-1, str_index_string(str, "b"));
    EXPECT_EQ(-1, str_index_string(str, "猫🍌d"));

    EXPECT_EQ(0, str_index_string(str, "\0"));
    EXPECT_EQ(0, str_index_string("\0", "\0"));
    EXPECT_EQ(-1, str_index_string("\0", "a"));

    str_free(str);
}

TEST(string_builder, StrSubstring) {
    char * str = str_of("a猫🍌c");
    char * sub;

    sub = str_substring(str, 1, 2);
    EXPECT_STREQ("猫", sub);

    sub = str_substring(str, 0, 4);
    EXPECT_STREQ("a猫🍌c", sub);

    sub = str_substring(str, 1, 10);
    EXPECT_STREQ("猫🍌c", sub);

    str_free(str);

    EXPECT_STREQ("a", str_substring("a", 0, 1));

    EXPECT_STREQ("", str_substring("a", 1, 2));

    EXPECT_STREQ("a", str_substring("a", 1, 0));

    EXPECT_STREQ("", str_substring("a", 0, 0));

    EXPECT_STREQ("", str_substring("a", 5, 5));

    EXPECT_STREQ("Hello", str_substring("Hello World", 0, 5));
    EXPECT_STREQ("World", str_substring("Hello World", 6, 11));

    EXPECT_STREQ("猫🍌", str_substring("猫🍌", 0, 2));
    EXPECT_STREQ("猫", str_substring(str_substring("猫🍌", 0, 2), 0, 1));
    EXPECT_STREQ("🍌", str_substring(str_substring("猫🍌", 1, 2), 0, 1));
}