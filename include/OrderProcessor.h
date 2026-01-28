#pragma once

namespace enterprise {
namespace business {

class OrderProcessor {
public:
    static bool processOrder(int);
};

class PaymentGateway {
public:
    static bool charge(double);
};

class InventoryManager {
public:
    static int getStock(int);
};

} // namespace business
} // namespace enterprise
