#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <variant>

namespace enterprise {
namespace database {

class Connection;
class Transaction;

using QueryParam = std::variant<std::string, int, double, bool, std::nullptr_t>;
using QueryResult = std::map<std::string, QueryParam>;

class QueryExecutor {
public:
    explicit QueryExecutor(std::shared_ptr<Connection> conn);
    ~QueryExecutor();
    
    // Execute queries
    bool execute(const std::string& sql);
    std::vector<QueryResult> executeQuery(const std::string& sql, 
                                          const std::vector<QueryParam>& params = {});
    int executeUpdate(const std::string& sql, 
                     const std::vector<QueryParam>& params = {});
    
    // Prepared statements
    bool prepare(const std::string& name, const std::string& sql);
    std::vector<QueryResult> executePrepared(const std::string& name,
                                            const std::vector<QueryParam>& params);
    
    // Transactions (raw pointer to avoid incomplete type issues)
    Transaction* beginTransaction();
    
    // Batch operations
    int executeBatch(const std::string& sql, 
                    const std::vector<std::vector<QueryParam>>& batchParams);
    
    // Query analysis
    std::string explainQuery(const std::string& sql);
    
private:
    std::string sanitizeParam(const QueryParam& param);
    bool validateSQL(const std::string& sql);
    
    std::shared_ptr<Connection> connection_;
    std::map<std::string, std::string> preparedStatements_;
};

} // namespace database
} // namespace enterprise
