#include <gtest/gtest.h>
#include "StringUtils.h"

using namespace enterprise::utils;

TEST(StringUtilsTest, Trim) {
    EXPECT_EQ(StringUtils::trim("  hello  "), "hello");
    EXPECT_EQ(StringUtils::trim("hello"), "hello");
    EXPECT_EQ(StringUtils::trim("   "), "");
}

TEST(StringUtilsTest, Split) {
    auto parts = StringUtils::split("a,b,c", ',');
    ASSERT_EQ(parts.size(), 3);
    EXPECT_EQ(parts[0], "a");
    EXPECT_EQ(parts[1], "b");
    EXPECT_EQ(parts[2], "c");
}

TEST(StringUtilsTest, Join) {
    std::vector<std::string> parts = {"a", "b", "c"};
    EXPECT_EQ(StringUtils::join(parts, ","), "a,b,c");
}

TEST(StringUtilsTest, CaseConversion) {
    EXPECT_EQ(StringUtils::toUpper("hello"), "HELLO");
    EXPECT_EQ(StringUtils::toLower("WORLD"), "world");
}

TEST(StringUtilsTest, Validation) {
    EXPECT_TRUE(StringUtils::isNumeric("12345"));
    EXPECT_FALSE(StringUtils::isNumeric("123a45"));
    
    EXPECT_TRUE(StringUtils::isEmail("test@example.com"));
    EXPECT_FALSE(StringUtils::isEmail("invalid-email"));
}
