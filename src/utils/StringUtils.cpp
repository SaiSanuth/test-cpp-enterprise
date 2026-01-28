#include "StringUtils.h"
#include <algorithm>
#include <regex>
#include <sstream>
#include <iomanip>

namespace enterprise {
namespace utils {

std::string StringUtils::trim(const std::string& str) {
    return ltrim(rtrim(str));
}

std::string StringUtils::ltrim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start);
}

std::string StringUtils::rtrim(const std::string& str) {
    size_t end = str.find_last_not_of(" \t\n\r");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

std::string StringUtils::toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string StringUtils::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool StringUtils::startsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() && 
           str.compare(0, prefix.size(), prefix) == 0;
}

bool StringUtils::endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::vector<std::string> StringUtils::split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    
    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    
    tokens.push_back(str.substr(start));
    return tokens;
}

std::string StringUtils::join(const std::vector<std::string>& strings, const std::string& separator) {
    if (strings.empty()) return "";
    
    std::ostringstream oss;
    oss << strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        oss << separator << strings[i];
    }
    return oss.str();
}

std::string StringUtils::replace(const std::string& str, const std::string& from, const std::string& to) {
    size_t pos = str.find(from);
    if (pos == std::string::npos) {
        return str;
    }
    std::string result = str;
    result.replace(pos, from.length(), to);
    return result;
}

std::string StringUtils::replaceAll(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    size_t pos = 0;
    
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    
    return result;
}

bool StringUtils::isNumeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

bool StringUtils::isAlphanumeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isalnum);
}

bool StringUtils::isEmail(const std::string& str) {
    static const std::regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(str, pattern);
}

bool StringUtils::isURL(const std::string& str) {
    static const std::regex pattern(R"(^https?://[^\s]+$)");
    return std::regex_match(str, pattern);
}

std::string StringUtils::urlEncode(const std::string& str) {
    std::ostringstream oss;
    oss << std::hex << std::uppercase;
    
    for (char c : str) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            oss << c;
        } else {
            oss << '%' << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
        }
    }
    
    return oss.str();
}

std::string StringUtils::urlDecode(const std::string& str) {
    std::string result;
    
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '%' && i + 2 < str.length()) {
            std::string hex = str.substr(i + 1, 2);
            char decoded = static_cast<char>(std::stoi(hex, nullptr, 16));
            result += decoded;
            i += 2;
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    
    return result;
}

static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string StringUtils::base64Encode(const std::vector<uint8_t>& data) {
    std::string result;
    int val = 0;
    int valb = -6;
    
    for (uint8_t c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    
    if (valb > -6) {
        result.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    
    while (result.size() % 4) {
        result.push_back('=');
    }
    
    return result;
}

std::vector<uint8_t> StringUtils::base64Decode(const std::string& str) {
    std::vector<uint8_t> result;
    int val = 0;
    int valb = -8;
    
    for (char c : str) {
        if (c == '=') break;
        
        size_t pos = base64_chars.find(c);
        if (pos == std::string::npos) continue;
        
        val = (val << 6) + pos;
        valb += 6;
        
        if (valb >= 0) {
            result.push_back((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    
    return result;
}

} // namespace utils
} // namespace enterprise
