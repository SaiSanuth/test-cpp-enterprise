#pragma once
#include <string>

namespace enterprise {
namespace auth {

class JWTManager {
public:
    static std::string generate(const std::string& payload);
    static bool verify(const std::string& token);
};

class PasswordHasher {
public:
    static std::string hash(const std::string& password);
};

class SessionManager {
public:
    SessionManager() = default;
};

} // namespace auth
} // namespace enterprise
