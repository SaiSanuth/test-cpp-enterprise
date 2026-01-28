#include "ConfigManager.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

namespace enterprise {
namespace core {

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open config file: " + filename);
        return false;
    }
    
    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        
        // Parse key=value
        size_t pos = line.find('=');
        if (pos == std::string::npos) {
            LOG_WARN("Invalid config line " + std::to_string(lineNumber) + ": " + line);
            continue;
        }
        
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        
        // Trim whitespace
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        
        // Try to parse as different types
        if (value == "true" || value == "false") {
            config_[key] = (value == "true");
        } else if (value.find('.') != std::string::npos) {
            try {
                config_[key] = std::stod(value);
            } catch (...) {
                config_[key] = value;
            }
        } else {
            try {
                config_[key] = std::stoi(value);
            } catch (...) {
                config_[key] = value;
            }
        }
    }
    
    LOG_INFO("Loaded " + std::to_string(config_.size()) + " config entries from " + filename);
    return true;
}

bool ConfigManager::loadFromEnvironment() {
    // Load common environment variables
    const char* env_vars[] = {
        "DATABASE_URL", "REDIS_URL", "PORT", "HOST",
        "LOG_LEVEL", "CACHE_SIZE", "MAX_CONNECTIONS"
    };
    
    for (const char* var : env_vars) {
        const char* value = std::getenv(var);
        if (value) {
            config_[var] = std::string(value);
            LOG_DEBUG("Loaded env var: " + std::string(var));
        }
    }
    
    return true;
}

bool ConfigManager::has(const std::string& key) const {
    return config_.find(key) != config_.end();
}

void ConfigManager::set(const std::string& key, const ConfigValue& value) {
    config_[key] = value;
}

void ConfigManager::remove(const std::string& key) {
    config_.erase(key);
}

std::string ConfigManager::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = config_.find(key);
    if (it == config_.end()) {
        return defaultValue;
    }
    if (auto* val = std::get_if<std::string>(&it->second)) {
        return *val;
    }
    return defaultValue;
}

int ConfigManager::getInt(const std::string& key, int defaultValue) const {
    auto it = config_.find(key);
    if (it == config_.end()) {
        return defaultValue;
    }
    if (auto* val = std::get_if<int>(&it->second)) {
        return *val;
    }
    return defaultValue;
}

double ConfigManager::getDouble(const std::string& key, double defaultValue) const {
    auto it = config_.find(key);
    if (it == config_.end()) {
        return defaultValue;
    }
    if (auto* val = std::get_if<double>(&it->second)) {
        return *val;
    }
    return defaultValue;
}

bool ConfigManager::getBool(const std::string& key, bool defaultValue) const {
    auto it = config_.find(key);
    if (it == config_.end()) {
        return defaultValue;
    }
    if (auto* val = std::get_if<bool>(&it->second)) {
        return *val;
    }
    return defaultValue;
}

void ConfigManager::dumpConfig() const {
    LOG_INFO("=== Configuration Dump ===");
    for (const auto& [key, value] : config_) {
        std::ostringstream oss;
        oss << key << " = ";
        std::visit([&oss](const auto& v) { oss << v; }, value);
        LOG_INFO(oss.str());
    }
}

} // namespace core
} // namespace enterprise
