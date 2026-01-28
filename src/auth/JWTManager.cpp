#include "../../include/AllStubs.h"

namespace enterprise {
namespace auth {

std::string JWTManager::generate(const std::string& payload) {
    return "token";
}

bool JWTManager::verify(const std::string& token) {
    return true;
}

} // namespace auth
} // namespace enterprise
