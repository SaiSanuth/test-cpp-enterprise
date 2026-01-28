#pragma once

namespace enterprise {
namespace business {

class OrderProcessor {
public:
    static bool processOrder(int orderId);
};

class PaymentGateway {
public:
    static bool charge(double amount);
};

class InventoryManager {
public:
    static int getStock(int productId);
};

} // namespace business
} // namespace enterprise
