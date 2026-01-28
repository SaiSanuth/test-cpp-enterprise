#pragma once
#include <list>
#include <unordered_map>
#include <mutex>
#include <optional>
#include <functional>

namespace enterprise {
namespace cache {

template<typename K, typename V>
class LRUCache {
public:
    explicit LRUCache(size_t capacity) 
        : capacity_(capacity), hits_(0), misses_(0) {}
    
    void put(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            // Update existing
            lruList_.erase(it->second.second);
            lruList_.push_front({key, value});
            it->second = {value, lruList_.begin()};
        } else {
            // Insert new
            if (cache_.size() >= capacity_) {
                // Evict LRU
                auto last = lruList_.back();
                cache_.erase(last.first);
                lruList_.pop_back();
            }
            lruList_.push_front({key, value});
            cache_[key] = {value, lruList_.begin()};
        }
    }
    
    std::optional<V> get(const K& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        auto it = cache_.find(key);
        if (it == cache_.end()) {
            ++misses_;
            return std::nullopt;
        }
        
        ++hits_;
        // Move to front (most recently used)
        lruList_.splice(lruList_.begin(), lruList_, it->second.second);
        return it->second.first;
    }
    
    bool contains(const K& key) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return cache_.find(key) != cache_.end();
    }
    
    void remove(const K& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            lruList_.erase(it->second.second);
            cache_.erase(it);
        }
    }
    
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        cache_.clear();
        lruList_.clear();
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return cache_.size();
    }
    
    size_t capacity() const { return capacity_; }
    
    // Statistics
    double getHitRate() const {
        size_t total = hits_ + misses_;
        return total > 0 ? static_cast<double>(hits_) / total : 0.0;
    }
    
    size_t getHits() const { return hits_; }
    size_t getMisses() const { return misses_; }
    
private:
    using ListItem = std::pair<K, V>;
    using ListIterator = typename std::list<ListItem>::iterator;
    
    size_t capacity_;
    std::list<ListItem> lruList_;
    std::unordered_map<K, std::pair<V, ListIterator>> cache_;
    mutable std::mutex mutex_;
    
    size_t hits_;
    size_t misses_;
};

} // namespace cache
} // namespace enterprise
