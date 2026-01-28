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
};

} // namespace enterprise
