#pragma once
#include <string>
#include <map>

namespace enterprise {

class Calculator {
public:
    Calculator();
    ~Calculator();

    int add(int a, int b) const;
    int subtract(int a, int b) const;
    int multiply(int a, int b) const;
    double divide(int a, int b) const;

    bool isPrime(int n) const;
    int factorial(int n) const;
    double power(double base, int exponent) const;
    
    // New: Performance optimization methods
    void enableCache(bool enable);
    void clearCache();
    int getLastResult() const;
    
    // Enterprise: Operation auditing for compliance
    static int getTotalOperations();
    static void resetOperationCount();
    
private:
    mutable bool cacheEnabled;
    mutable int lastResult;
    
    // Enterprise: Track operations for auditing (BUG: Resource leak)
    static int operationCount;
    static std::map<int, int>* auditCache;  // Raw pointer never freed
    
    void recordOperation(int result) const;
};

} // namespace enterprise