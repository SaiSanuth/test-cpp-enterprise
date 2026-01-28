#include <gtest/gtest.h>
#include "LRUCache.h"

using namespace enterprise::cache;

TEST(LRUCacheTest, BasicOperations) {
    LRUCache<int, std::string> cache(3);
    
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    
    EXPECT_EQ(cache.size(), 3);
    
    auto val = cache.get(1);
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(*val, "one");
}

TEST(LRUCacheTest, Eviction) {
    LRUCache<int, std::string> cache(2);
    
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");  // Should evict 1
    
    EXPECT_FALSE(cache.contains(1));
    EXPECT_TRUE(cache.contains(2));
    EXPECT_TRUE(cache.contains(3));
}

TEST(LRUCacheTest, HitRate) {
    LRUCache<int, std::string> cache(10);
    
    cache.put(1, "one");
    cache.get(1);  // hit
    cache.get(2);  // miss
    
    EXPECT_GT(cache.getHitRate(), 0.0);
    EXPECT_LT(cache.getHitRate(), 1.0);
}
