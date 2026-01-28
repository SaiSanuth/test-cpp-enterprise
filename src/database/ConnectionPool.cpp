#include "ConnectionPool.h"
#include "Logger.h"
#include <thread>
#include <stdexcept>

namespace enterprise {
namespace database {

Connection::Connection(int connId) 
    : id(connId)
    , inUse(false)
    , handle(nullptr)
    , lastUsed(std::chrono::system_clock::now()) {
    // Simulate connection creation
    handle = reinterpret_cast<void*>(static_cast<intptr_t>(connId));
}

Connection::~Connection() {
    // Cleanup connection
    handle = nullptr;
}

bool Connection::isValid() const {
    // Check if connection is still valid
    auto now = std::chrono::system_clock::now();
    auto age = std::chrono::duration_cast<std::chrono::minutes>(now - lastUsed);
    return age.count() < 30 && handle != nullptr;
}

void Connection::reset() {
    // Reset connection state
    lastUsed = std::chrono::system_clock::now();
}

ConnectionPool::ConnectionPool(const std::string& connectionString, 
                               size_t minConnections,
                               size_t maxConnections)
    : connectionString_(connectionString)
    , minConnections_(minConnections)
    , maxConnections_(maxConnections)
    , nextConnectionId_(1)
    , shutdownRequested_(false) {
    
    if (minConnections > maxConnections) {
        throw std::invalid_argument("minConnections cannot exceed maxConnections");
    }
    
    LOG_INFO("Creating connection pool: " + connectionString);
    
    // Create initial connections
    for (size_t i = 0; i < minConnections_; ++i) {
        auto conn = createConnection();
        connections_.push_back(conn);
        availableConnections_.push(conn);
    }
    
    LOG_INFO("Connection pool created with " + std::to_string(minConnections_) + " connections");
}

ConnectionPool::~ConnectionPool() {
    std::lock_guard<std::mutex> lock(mutex_);
    shutdownRequested_ = true;
    cv_.notify_all();
    
    connections_.clear();
    while (!availableConnections_.empty()) {
        availableConnections_.pop();
    }
    
    LOG_INFO("Connection pool destroyed");
}

std::shared_ptr<Connection> ConnectionPool::createConnection() {
    int id = nextConnectionId_++;
    LOG_DEBUG("Creating new connection with ID: " + std::to_string(id));
    return std::make_shared<Connection>(id);
}

std::shared_ptr<Connection> ConnectionPool::acquireConnection(int timeoutMs) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    auto deadline = std::chrono::steady_clock::now() + 
                    std::chrono::milliseconds(timeoutMs);
    
    while (true) {
        // Check if we have available connections
        if (!availableConnections_.empty()) {
            auto conn = availableConnections_.front();
            availableConnections_.pop();
            
            if (conn->isValid()) {
                conn->inUse = true;
                conn->reset();
                LOG_DEBUG("Acquired connection ID: " + std::to_string(conn->id));
                return conn;
            } else {
                // Connection is stale, remove it
                LOG_WARN("Removing stale connection ID: " + std::to_string(conn->id));
                auto it = std::find(connections_.begin(), connections_.end(), conn);
                if (it != connections_.end()) {
                    connections_.erase(it);
                }
                continue;
            }
        }
        
        // Try to create new connection if under limit
        if (connections_.size() < maxConnections_) {
            auto conn = createConnection();
            connections_.push_back(conn);
            conn->inUse = true;
            LOG_DEBUG("Created and acquired new connection ID: " + std::to_string(conn->id));
            return conn;
        }
        
        // Wait for a connection to be released
        if (cv_.wait_until(lock, deadline) == std::cv_status::timeout) {
            LOG_ERROR("Connection pool timeout after " + std::to_string(timeoutMs) + "ms");
            throw std::runtime_error("Connection pool timeout");
        }
        
        if (shutdownRequested_) {
            throw std::runtime_error("Connection pool is shutting down");
        }
    }
}

void ConnectionPool::releaseConnection(std::shared_ptr<Connection> conn) {
    if (!conn) {
        LOG_WARN("Attempted to release null connection");
        return;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    conn->inUse = false;
    conn->reset();
    availableConnections_.push(conn);
    
    LOG_DEBUG("Released connection ID: " + std::to_string(conn->id));
    cv_.notify_one();
}

size_t ConnectionPool::getActiveConnections() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return connections_.size() - availableConnections_.size();
}

size_t ConnectionPool::getIdleConnections() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return availableConnections_.size();
}

size_t ConnectionPool::getTotalConnections() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return connections_.size();
}

void ConnectionPool::shrinkPool() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    while (availableConnections_.size() > minConnections_ && 
           connections_.size() > minConnections_) {
        auto conn = availableConnections_.front();
        availableConnections_.pop();
        
        auto it = std::find(connections_.begin(), connections_.end(), conn);
        if (it != connections_.end()) {
            connections_.erase(it);
            LOG_DEBUG("Removed excess connection ID: " + std::to_string(conn->id));
        }
    }
}

void ConnectionPool::expandPool(size_t count) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    for (size_t i = 0; i < count && connections_.size() < maxConnections_; ++i) {
        auto conn = createConnection();
        connections_.push_back(conn);
        availableConnections_.push(conn);
    }
    
    cv_.notify_all();
}

bool ConnectionPool::healthCheck() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    bool allHealthy = true;
    for (const auto& conn : connections_) {
        if (!conn->isValid()) {
            LOG_WARN("Unhealthy connection detected: ID " + std::to_string(conn->id));
            allHealthy = false;
        }
    }
    
    return allHealthy;
}

void ConnectionPool::removeStaleConnections() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = connections_.begin();
    while (it != connections_.end()) {
        if (!(*it)->isValid() && !(*it)->inUse) {
            LOG_INFO("Removing stale connection ID: " + std::to_string((*it)->id));
            it = connections_.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace database
} // namespace enterprise
