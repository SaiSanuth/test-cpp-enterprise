#pragma once

namespace enterprise {
namespace network {

class TcpServer {
public:
    TcpServer(int port) {}
    ~TcpServer() = default;
};

class HttpClient {
public:
    static std::string get(const std::string& url) { return ""; }
};

class WebSocketHandler {
public:
    WebSocketHandler() = default;
};

} // namespace network
} // namespace enterprise
