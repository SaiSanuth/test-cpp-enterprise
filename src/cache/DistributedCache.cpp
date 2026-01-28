#include "DistributedCache.h"
#include "Logger.h"
#include <functional>

namespace enterprise {
namespace cache {

DistributedCache::DistributedCache(const std::vector<CacheNode>& nodes, size_t localCacheSize)
    : nodes_(nodes) {
    localCache_ = std::make_unique<LRUCache<std::string, std::string>>(localCacheSize);
    LOG_INFO("Distributed cache initialized with " + std::to_string(nodes.size()) + " nodes");
}

DistributedCache::~DistributedCache() = default;

size_t DistributedCache::getNodeForKey(const std::string& key) const {
    std::hash<std::string> hasher;
    return hasher(key) % nodes_.size();
}

bool DistributedCache::put(const std::string& key, const std::string& value, int ttlSeconds) {
    localCache_->put(key, value);
    return syncToRemote(key, value);
}

std::optional<std::string> DistributedCache::get(const std::string& key) {
    auto localResult = localCache_->get(key);
    if (localResult) {
        ++stats_.localHits;
        return localResult;
    }
    
    auto remoteResult = fetchFromRemote(key);
    if (remoteResult) {
        ++stats_.remoteHits;
        localCache_->put(key, *remoteResult);
        return remoteResult;
    }
    
    ++stats_.misses;
    return std::nullopt;
}

bool DistributedCache::remove(const std::string& key) {
    localCache_->remove(key);
    return true;
}

void DistributedCache::putBatch(const std::map<std::string, std::string>& items) {
    for (const auto& [key, value] : items) {
        put(key, value);
    }
}

std::map<std::string, std::string> DistributedCache::getBatch(const std::vector<std::string>& keys) {
    std::map<std::string, std::string> result;
    for (const auto& key : keys) {
        auto value = get(key);
        if (value) {
            result[key] = *value;
        }
    }
    return result;
}

void DistributedCache::addNode(const CacheNode& node) {
    nodes_.push_back(node);
}

void DistributedCache::removeNode(const std::string& address, int port) {
    nodes_.erase(
        std::remove_if(nodes_.begin(), nodes_.end(),
            [&](const CacheNode& n) { return n.address == address && n.port == port; }),
        nodes_.end());
}

std::vector<CacheNode> DistributedCache::getActiveNodes() const {
    std::vector<CacheNode> active;
    for (const auto& node : nodes_) {
        if (node.available) {
            active.push_back(node);
        }
    }
    return active;
}

void DistributedCache::healthCheck() {
    for (auto& node : nodes_) {
        node.lastPing = std::chrono::system_clock::now();
        node.available = true; // Simplified
    }
}

size_t DistributedCache::getNodeCount() const {
    return nodes_.size();
}

DistributedCache::Stats DistributedCache::getStatistics() const {
    return stats_;
}

bool DistributedCache::syncToRemote(const std::string& key, const std::string& value) {
    // Simulated remote sync
    return true;
}

std::optional<std::string> DistributedCache::fetchFromRemote(const std::string& key) {
    // Simulated remote fetch
    return std::nullopt;
}

} // namespace cache
} // namespace enterprise
