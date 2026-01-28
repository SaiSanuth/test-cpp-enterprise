#pragma once
#include <string>
#include <map>
#include <memory>
#include <variant>
#include <optional>

namespace enterprise {
namespace core {

using ConfigValue = std::variant<std::string, int, double, bool>;

class ConfigManager {
public:
    static ConfigManager& getInstance();
    
    bool loadFromFile(const std::string& filename);
    bool loadFromEnvironment();
    
    template<typename T>
    std::optional<T> get(const std::string& key) const;
    
    void set(const std::string& key, const ConfigValue& value);
    bool has(const std::string& key) const;
    void remove(const std::string& key);
    
    // Special getters with defaults
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    int getInt(const std::string& key, int defaultValue = 0) const;
    double getDouble(const std::string& key, double defaultValue = 0.0) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    
    void dumpConfig() const;
    
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
private:
    ConfigManager() = default;
    std::map<std::string, ConfigValue> config_;
};

} // namespace core
} // namespace enterprise
