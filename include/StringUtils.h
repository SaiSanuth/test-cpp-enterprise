#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdint>

namespace enterprise {
namespace utils {

class StringUtils {
public:
    // String manipulation
    static std::string trim(const std::string& str);
    static std::string ltrim(const std::string& str);
    static std::string rtrim(const std::string& str);
    
    static std::string toUpper(const std::string& str);
    static std::string toLower(const std::string& str);
    
    static bool startsWith(const std::string& str, const std::string& prefix);
    static bool endsWith(const std::string& str, const std::string& suffix);
    
    // Splitting and joining
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
    static std::string join(const std::vector<std::string>& strings, const std::string& separator);
    
    // Search and replace
    static std::string replace(const std::string& str, const std::string& from, const std::string& to);
    static std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);
    
    // Validation
    static bool isNumeric(const std::string& str);
    static bool isAlphanumeric(const std::string& str);
    static bool isEmail(const std::string& str);
    static bool isURL(const std::string& str);
    
    // Encoding
    static std::string urlEncode(const std::string& str);
    static std::string urlDecode(const std::string& str);
    static std::string base64Encode(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> base64Decode(const std::string& str);
    
    // Formatting
    template<typename... Args>
    static std::string format(const std::string& fmt, Args... args) {
        size_t size = std::snprintf(nullptr, 0, fmt.c_str(), args...) + 1;
        std::vector<char> buf(size);
        std::snprintf(buf.data(), size, fmt.c_str(), args...);
        return std::string(buf.data(), buf.data() + size - 1);
    }
};

} // namespace utils
} // namespace enterprise
