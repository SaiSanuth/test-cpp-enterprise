#pragma once
#include <string>

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
    
private:
    mutable bool cacheEnabled;
    mutable int lastResult;
};

} // namespace enterprise