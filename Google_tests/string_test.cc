#include "gtest/gtest.h"
#include "../string.h"

TEST(string_builder, StrOfEmpty) {
    char * str = str_of_empty();

    EXPECT_EQ(0, str_len(str));
    EXPECT_EQ(0, str_size(str));
    EXPECT_STREQ("", str);

    str = str_free(str);
    EXPECT_EQ(nullptr, str);
}

TEST(string_builder, StrOf) {
    char * str = str_of("a猫🍌");

    EXPECT_EQ(3, str_len(str));
    EXPECT_EQ(8, str_size(str)); // Doesn't count the '\0' byte
    EXPECT_STREQ("a猫🍌", str);

    str = str_free(str);
    EXPECT_EQ(nullptr, str);

    str = str_of("");

    EXPECT_EQ(0, str_len(str));
    EXPECT_EQ(0, str_size(str));
    EXPECT_STREQ("", str);

    str = str_free(str);
    EXPECT_EQ(nullptr, str);

    str = str_of(nullptr);

    EXPECT_EQ(0, str_len(str));
    EXPECT_EQ(0, str_size(str));
    EXPECT_STREQ("", str);

    str = str_free(str);
    EXPECT_EQ(nullptr, str);
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

    str = str_free(str);
    EXPECT_EQ(nullptr, str);
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

    str = str_free(str);
    EXPECT_EQ(nullptr, str);
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

    str = str_free(str);
    EXPECT_EQ(nullptr, str);
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

    str = str_free(str);
    EXPECT_EQ(nullptr, str);

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