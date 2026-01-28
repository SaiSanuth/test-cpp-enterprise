#pragma once
#include <string>

namespace enterprise {
namespace network {

class TcpServer {
public:
    TcpServer(int) {}  // Unnamed parameter to avoid unused warning
    ~TcpServer() = default;
};

class HttpClient {
public:
    static std::string get(const std::string&) { return ""; }  // Unnamed parameter
};

class WebSocketHandler {
public:
    WebSocketHandler() = default;
};

} // namespace network
} // namespace enterprise
