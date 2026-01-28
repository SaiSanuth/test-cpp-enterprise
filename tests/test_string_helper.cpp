#include <gtest/gtest.h>
#include "StringHelper.h"

using namespace enterprise;

TEST(StringHelperTest, ToUpper) {
    EXPECT_EQ(StringHelper::toUpper("hello"), "HELLO");
    EXPECT_EQ(StringHelper::toUpper("Hello World"), "HELLO WORLD");
    EXPECT_EQ(StringHelper::toUpper(""), "");
}

TEST(StringHelperTest, ToLower) {
    EXPECT_EQ(StringHelper::toLower("HELLO"), "hello");
    EXPECT_EQ(StringHelper::toLower("Hello World"), "hello world");
    EXPECT_EQ(StringHelper::toLower(""), "");
}

TEST(StringHelperTest, Trim) {
    EXPECT_EQ(StringHelper::trim("  hello  "), "hello");
    EXPECT_EQ(StringHelper::trim("\t\nworld\r\n"), "world");
    EXPECT_EQ(StringHelper::trim("no-spaces"), "no-spaces");
    EXPECT_EQ(StringHelper::trim("   "), "");
}

TEST(StringHelperTest, Split) {
    auto result = StringHelper::split("a,b,c", ',');
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");
    
    auto single = StringHelper::split("hello", ',');
    ASSERT_EQ(single.size(), 1);
    EXPECT_EQ(single[0], "hello");
}

TEST(StringHelperTest, Join) {
    std::vector<std::string> parts = {"a", "b", "c"};
    EXPECT_EQ(StringHelper::join(parts, ","), "a,b,c");
    EXPECT_EQ(StringHelper::join(parts, " "), "a b c");
    
    std::vector<std::string> empty;
    EXPECT_EQ(StringHelper::join(empty, ","), "");
}

TEST(StringHelperTest, StartsWith) {
    EXPECT_TRUE(StringHelper::startsWith("hello world", "hello"));
    EXPECT_FALSE(StringHelper::startsWith("hello world", "world"));
    EXPECT_TRUE(StringHelper::startsWith("test", "test"));
    EXPECT_FALSE(StringHelper::startsWith("short", "longer"));
}

TEST(StringHelperTest, EndsWith) {
    EXPECT_TRUE(StringHelper::endsWith("hello world", "world"));
    EXPECT_FALSE(StringHelper::endsWith("hello world", "hello"));
    EXPECT_TRUE(StringHelper::endsWith("test", "test"));
    EXPECT_FALSE(StringHelper::endsWith("short", "longer"));
}

TEST(StringHelperTest, Reverse) {
    EXPECT_EQ(StringHelper::reverse("hello"), "olleh");
    EXPECT_EQ(StringHelper::reverse("a"), "a");
    EXPECT_EQ(StringHelper::reverse(""), "");
    EXPECT_EQ(StringHelper::reverse("12345"), "54321");
}
