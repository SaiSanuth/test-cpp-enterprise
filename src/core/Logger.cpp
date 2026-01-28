#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace enterprise {
namespace core {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() 
    : minLevel_(LogLevel::INFO)
    , consoleOutput_(true)
    , totalLogs_(0)
    , errorCount_(0) {
}

Logger::~Logger() {
    flush();
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::log(LogLevel level, const std::string& message, 
                 const std::string& file, int line) {
    if (level < minLevel_) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::string formatted = formatMessage(level, message, file, line);
    
    if (consoleOutput_) {
        if (level >= LogLevel::ERROR) {
            std::cerr << formatted << std::endl;
        } else {
            std::cout << formatted << std::endl;
        }
    }
    
    if (logFile_.is_open()) {
        logFile_ << formatted << std::endl;
    }
    
    ++totalLogs_;
    if (level >= LogLevel::ERROR) {
        ++errorCount_;
    }
}

void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    minLevel_ = level;
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (logFile_.is_open()) {
        logFile_.close();
    }
    logFile_.open(filename, std::ios::app);
}

void Logger::enableConsoleOutput(bool enable) {
    consoleOutput_ = enable;
}

void Logger::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (logFile_.is_open()) {
        logFile_.flush();
    }
}

size_t Logger::getTotalLogCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return totalLogs_;
}

size_t Logger::getErrorCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return errorCount_;
}

std::string Logger::formatMessage(LogLevel level, const std::string& message,
                                  const std::string& file, int line) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    oss << "." << std::setfill('0') << std::setw(3) << ms.count() << "] ";
    oss << "[" << logLevelToString(level) << "] ";
    
    if (!file.empty()) {
        // Extract just the filename
        size_t pos = file.find_last_of("/\\");
        std::string filename = (pos != std::string::npos) ? file.substr(pos + 1) : file;
        oss << filename << ":" << line << " - ";
    }
    
    oss << message;
    return oss.str();
}

std::string Logger::logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARN";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRIT";
        default: return "UNKNOWN";
    }
}

} // namespace core
} // namespace enterprise
