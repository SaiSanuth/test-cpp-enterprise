#include <gtest/gtest.h>
#include "ConnectionPool.h"

using namespace enterprise::database;

TEST(ConnectionPoolTest, Creation) {
    ConnectionPool pool("localhost:5432", 2, 10);
    EXPECT_EQ(pool.getTotalConnections(), 2);
    EXPECT_EQ(pool.getIdleConnections(), 2);
}

TEST(ConnectionPoolTest, AcquireRelease) {
    ConnectionPool pool("localhost:5432", 2, 10);
    
    auto conn = pool.acquireConnection();
    EXPECT_NE(conn, nullptr);
    EXPECT_EQ(pool.getActiveConnections(), 1);
    
    pool.releaseConnection(conn);
    EXPECT_EQ(pool.getActiveConnections(), 0);
}

TEST(ConnectionPoolTest, MaxConnections) {
    ConnectionPool pool("localhost:5432", 2, 3);
    
    auto conn1 = pool.acquireConnection();
    auto conn2 = pool.acquireConnection();
    auto conn3 = pool.acquireConnection();
    
    EXPECT_EQ(pool.getTotalConnections(), 3);
    EXPECT_EQ(pool.getActiveConnections(), 3);
}
