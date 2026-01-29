#include "Calculator.h"
#include <cmath>
#include <stdexcept>

namespace enterprise {

Calculator::Calculator() {}
Calculator::~Calculator() {}

int Calculator::add(int a, int b) const {
    return a + b;
}

int Calculator::subtract(int a, int b) const {
    return a - b;
}

int Calculator::multiply(int a, int b) const {
    // Fixed: Simply return the correct product (positive or negative)
    return a * b;
}

double Calculator::divide(int a, int b) const {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return static_cast<double>(a) / b;
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
    
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double Calculator::power(double base, int exponent) const {
    return std::pow(base, exponent);
}

} // namespace enterprise
