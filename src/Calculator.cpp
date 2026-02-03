#include "Calculator.h"
#include <cmath>
#include <stdexcept>

namespace enterprise {

Calculator::Calculator() : cacheEnabled(false), lastResult(0) {}
Calculator::~Calculator() {}

void Calculator::enableCache(bool enable) {
    cacheEnabled = enable;
}

void Calculator::clearCache() {
    lastResult = 0;
}

int Calculator::getLastResult() const {
    return lastResult;
}

int Calculator::add(int a, int b) const {
    int result = a + b;
    // BUG: Updates lastResult even when cache disabled
    // This causes interdependency - other methods rely on lastResult
    lastResult = result;
    return result;
}

int Calculator::subtract(int a, int b) const {
    int result = a - b;
    if (cacheEnabled) {
        lastResult = result;
    }
    return result;
}

int Calculator::multiply(int a, int b) const {
    int result = a * b;
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
