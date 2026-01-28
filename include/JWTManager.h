#pragma once
#include <string>

namespace enterprise {
namespace auth {

class JWTManager {
public:
    static std::string generate(const std::string&);
    static bool verify(const std::string&);
};

class PasswordHasher {
public:
    static std::string hash(const std::string&);
};

class SessionManager {
public:
    SessionManager() = default;
};

} // namespace auth
} // namespace enterprise
