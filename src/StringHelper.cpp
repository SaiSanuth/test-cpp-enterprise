#include "StringHelper.h"
#include <algorithm>
#include <sstream>

namespace enterprise {

StringHelper::StringHelper() {}
StringHelper::~StringHelper() {}

std::string StringHelper::toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), 
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return result;
}

std::string StringHelper::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), 
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

std::string StringHelper::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> StringHelper::split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    
    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

std::string StringHelper::join(const std::vector<std::string>& parts, const std::string& separator) {
    if (parts.empty()) return "";
    
    std::string result = parts[0];
    for (size_t i = 1; i < parts.size(); i++) {
        result += separator + parts[i];
    }
    return result;
}

bool StringHelper::startsWith(const std::string& str, const std::string& prefix) {
    if (prefix.length() > str.length()) return false;
    return str.compare(0, prefix.length(), prefix) == 0;
}

bool StringHelper::endsWith(const std::string& str, const std::string& suffix) {
    if (suffix.length() > str.length()) return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

std::string StringHelper::reverse(const std::string& str) {
    return std::string(str.rbegin(), str.rend());
}

} // namespace enterprise
