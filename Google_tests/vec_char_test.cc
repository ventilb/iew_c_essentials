#include "gtest/gtest.h"
#include "../vec_char.h"

TEST(vec_char, StringTest) {
    vec_char vec = vec_char_new();

    EXPECT_EQ(COL_OK, vec_char_push_back(vec, 'H'));
    EXPECT_EQ(COL_OK, vec_char_push_back(vec, 'e'));
    EXPECT_EQ(COL_OK, vec_char_push_back(vec, 'l'));
    EXPECT_EQ(COL_OK, vec_char_push_back(vec, 'l'));
    EXPECT_EQ(COL_OK, vec_char_push_back(vec, 'o'));
    EXPECT_EQ(COL_OK, vec_char_push_back(vec, ' '));
    EXPECT_EQ(COL_OK, vec_char_push_back(vec, 'W'));
    EXPECT_EQ(COL_OK, vec_char_push_back(vec, 'o'));
    EXPECT_EQ(COL_OK, vec_char_push_back(vec, 'r'));
    EXPECT_EQ(COL_OK, vec_char_push_back(vec, 'l'));
    EXPECT_EQ(COL_OK, vec_char_push_back(vec, 'd'));
    EXPECT_EQ(COL_OK, vec_char_push_back(vec, '\0'));

    EXPECT_EQ(12, vec_char_len(vec)); // includes '\0'
    EXPECT_EQ(11, strlen(vec_char_data(vec))); // excludes '\0'
    EXPECT_STRCASEEQ("Hello World", vec_char_data(vec));
}

