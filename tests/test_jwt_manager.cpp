#include <gtest/gtest.h>
#include "../include/AllStubs.h"

using namespace enterprise::auth;

TEST(JWTManagerTest, GenerateToken) {
    JWTManager jwt;
    std::string token = jwt.generate("user123");
    EXPECT_FALSE(token.empty());
}

TEST(JWTManagerTest, VerifyToken) {
    JWTManager jwt;
    std::string token = jwt.generate("user123");
    EXPECT_TRUE(jwt.verify(token));
}
