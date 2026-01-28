#include <gtest/gtest.h>
#include "Logger.h"

using namespace enterprise::core;

TEST(LoggerTest, SingletonInstance) {
    Logger& logger1 = Logger::getInstance();
    Logger& logger2 = Logger::getInstance();
    EXPECT_EQ(&logger1, &logger2);
}

TEST(LoggerTest, LogLevels) {
    Logger& logger = Logger::getInstance();
    logger.setLogLevel(LogLevel::WARNING);
    
    size_t initialCount = logger.getTotalLogCount();
    
    LOG_DEBUG("This should not be logged");
    EXPECT_EQ(logger.getTotalLogCount(), initialCount);
    
    LOG_ERROR("This should be logged");
    EXPECT_EQ(logger.getTotalLogCount(), initialCount + 1);
}

TEST(LoggerTest, ErrorCounting) {
    Logger& logger = Logger::getInstance();
    size_t initialErrors = logger.getErrorCount();
    
    LOG_ERROR("Test error");
    EXPECT_EQ(logger.getErrorCount(), initialErrors + 1);
    
    LOG_INFO("Test info");
    EXPECT_EQ(logger.getErrorCount(), initialErrors + 1);
}
