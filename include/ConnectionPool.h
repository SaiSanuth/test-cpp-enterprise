#pragma once
#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace enterprise {
namespace database {

struct Connection {
    int id;
    bool inUse;
    void* handle;  // Simulated database handle
    std::chrono::system_clock::time_point lastUsed;
    
    Connection(int connId);
    ~Connection();
    
    bool isValid() const;
    void reset();
};

class ConnectionPool {
public:
    ConnectionPool(const std::string& connectionString, 
                   size_t minConnections = 5,
                   size_t maxConnections = 20);
    ~ConnectionPool();
    
    std::shared_ptr<Connection> acquireConnection(int timeoutMs = 5000);
    void releaseConnection(std::shared_ptr<Connection> conn);
    
    size_t getActiveConnections() const;
    size_t getIdleConnections() const;
    size_t getTotalConnections() const;
    
    void shrinkPool();
    void expandPool(size_t count);
    
    // Health check
    bool healthCheck();
    void removeStaleConnections();
    
private:
    std::shared_ptr<Connection> createConnection();
    void maintainPool();
    
    std::string connectionString_;
    size_t minConnections_;
    size_t maxConnections_;
    
    std::vector<std::shared_ptr<Connection>> connections_;
    std::queue<std::shared_ptr<Connection>> availableConnections_;
    
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    
    int nextConnectionId_;
    bool shutdownRequested_;
};

} // namespace database
} // namespace enterprise
