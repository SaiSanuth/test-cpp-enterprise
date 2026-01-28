#include "Transaction.h"
#include "Logger.h"

namespace enterprise {
namespace database {

Transaction::Transaction(std::shared_ptr<Connection> conn, IsolationLevel level)
    : connection_(conn), level_(level), active_(true), committed_(false) {
    LOG_DEBUG("Transaction started");
}

Transaction::~Transaction() {
    if (active_ && !committed_) {
        rollback();
    }
}

void Transaction::commit() {
    if (!active_) {
        throw std::runtime_error("Transaction is not active");
    }
    LOG_INFO("Transaction committed");
    committed_ = true;
    active_ = false;
}

void Transaction::rollback() {
    if (!active_) {
        throw std::runtime_error("Transaction is not active");
    }
    LOG_INFO("Transaction rolled back");
    active_ = false;
}

bool Transaction::isActive() const {
    return active_;
}

IsolationLevel Transaction::getIsolationLevel() const {
    return level_;
}

void Transaction::createSavepoint(const std::string& name) {
    LOG_DEBUG("Savepoint created: " + name);
}

void Transaction::rollbackToSavepoint(const std::string& name) {
    LOG_DEBUG("Rolled back to savepoint: " + name);
}

void Transaction::releaseSavepoint(const std::string& name) {
    LOG_DEBUG("Savepoint released: " + name);
}

} // namespace database
} // namespace enterprise
