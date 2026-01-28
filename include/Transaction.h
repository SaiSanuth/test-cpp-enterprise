#pragma once
#include <memory>
#include <functional>

namespace enterprise {
namespace database {

class Connection;

enum class IsolationLevel {
    READ_UNCOMMITTED,
    READ_COMMITTED,
    REPEATABLE_READ,
    SERIALIZABLE
};

class Transaction {
public:
    Transaction(std::shared_ptr<Connection> conn, IsolationLevel level);
    ~Transaction();
    
    void commit();
    void rollback();
    
    bool isActive() const;
    IsolationLevel getIsolationLevel() const;
    
    // Savepoints
    void createSavepoint(const std::string& name);
    void rollbackToSavepoint(const std::string& name);
    void releaseSavepoint(const std::string& name);
    
    // RAII helper
    class Guard {
    public:
        explicit Guard(Transaction& txn) : txn_(txn), committed_(false) {}
        ~Guard() { if (!committed_) txn_.rollback(); }
        void commit() { txn_.commit(); committed_ = true; }
    private:
        Transaction& txn_;
        bool committed_;
    };
    
private:
    std::shared_ptr<Connection> connection_;
    IsolationLevel level_;
    bool active_;
    bool committed_;
};

} // namespace database
} // namespace enterprise
