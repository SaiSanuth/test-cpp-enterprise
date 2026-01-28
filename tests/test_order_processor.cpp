#include <gtest/gtest.h>
#include "../include/AllStubs.h"

using namespace enterprise::business;

TEST(OrderProcessorTest, ProcessOrder) {
    OrderProcessor processor;
    EXPECT_TRUE(processor.processOrder(123));
}

TEST(PaymentGatewayTest, Charge) {
    PaymentGateway gateway;
    EXPECT_TRUE(gateway.charge(99.99));
}

TEST(InventoryManagerTest, GetStock) {
    InventoryManager inventory;
    EXPECT_GT(inventory.getStock(1), 0);
}
