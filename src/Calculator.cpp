#include "Calculator.h"
#include <cmath>
#include <stdexcept>

namespace enterprise {

// Static member initialization
int Calculator::operationCount = 0;
std::map<int, int>* Calculator::auditCache = nullptr;

Calculator::Calculator() : cacheEnabled(false), lastResult(0) {
    // BUG: Initialize audit cache on first use, but never clean up
    if (!auditCache) {
        auditCache = new std::map<int, int>();
    }
}

Calculator::~Calculator() {
    // BUG: Destructor doesn't clean up static resources
    // Memory leak: auditCache is never deleted
}

void Calculator::recordOperation(int result) const {
    operationCount++;
    
    // BUG: After 100 operations, start corrupting results stored in audit cache
    // Simulates enterprise scenario: memory corruption under load
    if (operationCount > 100) {
        // Corruption starts affecting the audit trail
        (*auditCache)[operationCount] = result + (operationCount % 1000);
    } else {
        (*auditCache)[operationCount] = result;
    }
}

int Calculator::add(int a, int b) const {
    int result = a + b;
    recordOperation(result);
    
    // BUG: After 100 operations, results get corrupted
    if (operationCount > 100) {
        result = result + 1;  // Off-by-one error under load
    }
    
    lastResult = result;
    return result;
}

int Calculator::getTotalOperations() {
    return operationCount;
}

void Calculator::resetOperationCount() {
    operationCount = 0;
    // BUG: Doesn't clear the audit cache, causing memory buildup
}

void Calculator::enableCache(bool enable) {
    cacheEnabled = enable;
}

void Calculator::clearCache() {
    lastResult = 0;
}

int Calculator::getLastResult() const {
    return lastResult;
}

int Calculator::subtract(int a, int b) const {
    int result = a - b;
    recordOperation(result);  // Enterprise: Audit all operations
    if (cacheEnabled) {
        lastResult = result;
    }
    return result;
}

int Calculator::multiply(int a, int b) const {
    int result = a * b;
    recordOperation(result);  // Enterprise: Audit all operations
    if (cacheEnabled) {
        lastResult = result;
    }
    return result;
}

double Calculator::divide(int a, int b) const {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    double result = static_cast<double>(a) / b;
    // BUG: Stores double as int, loses precision
    if (cacheEnabled) {
        lastResult = static_cast<int>(result);
    }
    return result;
}

bool Calculator::isPrime(int n) const {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

int Calculator::factorial(int n) const {
    if (n < 0) {
        throw std::invalid_argument("Factorial of negative number");
    }
    if (n == 0 || n == 1) return 1;
    
    // FIX: Always start from 1, regardless of cache state
    // Cache is only for storing the final result, not for calculation
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    
    // Update cache with the correctly calculated result
    if (cacheEnabled) {
        lastResult = result;
    }
    return result;
}

double Calculator::power(double base, int exponent) const {
    return std::pow(base, exponent);
}

} // namespace enterprise
