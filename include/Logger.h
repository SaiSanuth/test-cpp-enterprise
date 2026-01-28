#pragma once
#include <string>
#include <memory>
#include <chrono>
#include <fstream>
#include <mutex>
#include <vector>

namespace enterprise {
namespace core {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger {
public:
    static Logger& getInstance();
    
    void log(LogLevel level, const std::string& message, 
             const std::string& file = "", int line = 0);
    void setLogLevel(LogLevel level);
    void setLogFile(const std::string& filename);
    void enableConsoleOutput(bool enable);
    void flush();
    
    // Performance metrics
    size_t getTotalLogCount() const;
    size_t getErrorCount() const;
    
    // Prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
private:
    Logger();
    ~Logger();
    
    std::string formatMessage(LogLevel level, const std::string& message,
                              const std::string& file, int line);
    std::string logLevelToString(LogLevel level);
    
    LogLevel minLevel_;
    std::ofstream logFile_;
    bool consoleOutput_;
    mutable std::mutex mutex_;
    size_t totalLogs_;
    size_t errorCount_;
};

// Convenience macros
#define LOG_DEBUG(msg) enterprise::core::Logger::getInstance().log(enterprise::core::LogLevel::DEBUG, msg, __FILE__, __LINE__)
#define LOG_INFO(msg) enterprise::core::Logger::getInstance().log(enterprise::core::LogLevel::INFO, msg, __FILE__, __LINE__)
#define LOG_WARN(msg) enterprise::core::Logger::getInstance().log(enterprise::core::LogLevel::WARNING, msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) enterprise::core::Logger::getInstance().log(enterprise::core::LogLevel::ERROR, msg, __FILE__, __LINE__)
#define LOG_CRITICAL(msg) enterprise::core::Logger::getInstance().log(enterprise::core::LogLevel::CRITICAL, msg, __FILE__, __LINE__)

} // namespace core
} // namespace enterprise
