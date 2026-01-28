#pragma once
#include <string>
#include <fstream>
#include <mutex>

namespace enterprise {

class Logger {
public:
    enum Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };
    
    static Logger& getInstance();
    
    void log(Level level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    
private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    std::string levelToString(Level level) const;
    std::mutex mutex_;
};

} // namespace enterprise
