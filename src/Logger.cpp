#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

namespace enterprise {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {}
Logger::~Logger() {}

void Logger::log(Level level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    #ifdef _WIN32
        std::tm timeinfo;
        localtime_s(&timeinfo, &time);
        std::cout << "[" << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << "] "
                  << "[" << levelToString(level) << "] "
                  << message << std::endl;
    #else
        std::cout << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "] "
                  << "[" << levelToString(level) << "] "
                  << message << std::endl;
    #endif
}

void Logger::debug(const std::string& message) {
    log(DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(INFO, message);
}

void Logger::warning(const std::string& message) {
    log(WARNING, message);
}

void Logger::error(const std::string& message) {
    log(ERROR, message);
}

std::string Logger::levelToString(Level level) const {
    switch(level) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

} // namespace enterprise
