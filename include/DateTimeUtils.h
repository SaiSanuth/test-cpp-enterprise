#pragma once
#include <string>

namespace enterprise {
namespace utils {

class DateTimeUtils {
public:
    static std::string now();
};

class CryptoUtils {
public:
    static std::string hash(const std::string&);
};

} // namespace utils
} // namespace enterprise
