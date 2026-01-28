#pragma once
#include "LRUCache.h"
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <optional>
#include <map>

namespace enterprise {
namespace cache {

struct CacheNode {
    std::string address;
    int port;
    bool available;
    std::chrono::system_clock::time_point lastPing;
};

class DistributedCache {
public:
    DistributedCache(const std::vector<CacheNode>& nodes, size_t localCacheSize = 1000) 
        : nodes_(nodes) {
        localCache_ = std::unique_ptr<LRUCache<std::string, std::string>>(
            new LRUCache<std::string, std::string>(localCacheSize));
        stats_ = {};
    }
    
    ~DistributedCache() {
        localCache_.reset();
    }
    
    // Cache operations
    bool put(const std::string& key, const std::string& value, int ttlSeconds = 0);
    std::optional<std::string> get(const std::string& key);
    bool remove(const std::string& key);
    
    // Batch operations
    void putBatch(const std::map<std::string, std::string>& items);
    std::map<std::string, std::string> getBatch(const std::vector<std::string>& keys);
    
    // Node management
    void addNode(const CacheNode& node);
    void removeNode(const std::string& address, int port);
    std::vector<CacheNode> getActiveNodes() const;
    
    // Health monitoring
    void healthCheck();
    size_t getNodeCount() const;
    
    // Statistics
    struct Stats {
        size_t localHits;
        size_t remoteHits;
        size_t misses;
        size_t networkErrors;
    };
    Stats getStatistics() const;
    
private:
    size_t getNodeForKey(const std::string& key) const;
    bool syncToRemote(const std::string& key, const std::string& value);
    std::optional<std::string> fetchFromRemote(const std::string& key);
    
    std::vector<CacheNode> nodes_;
    std::unique_ptr<LRUCache<std::string, std::string>> localCache_;
    
    Stats stats_;
    mutable std::mutex mutex_;
};

} // namespace cache
} // namespace enterprise
