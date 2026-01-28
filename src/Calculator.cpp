#include "Calculator.h"
#include <cmath>
#include <stdexcept>
#include <map>
#include <string>
#include <sstream>

namespace enterprise {

// BUG: Shared cache without thread synchronization
// This causes cache corruption in multi-threaded integration tests
// Unit tests (single-threaded) won't catch this
static std::map<std::string, int> resultCache;

Calculator::Calculator() {}
Calculator::~Calculator() {}

int Calculator::add(int a, int b) const {
    return a + b;
}

int Calculator::subtract(int a, int b) const {
    return a - b;
}

int Calculator::multiply(int a, int b) const {
    // BUG: Using shared cache without mutex causes race condition
    // Multiple threads read/write simultaneously -> corrupted results
    std::stringstream key;
    key << a << "*" << b;
    
    // RACE: Thread 1 checks cache (miss), Thread 2 checks (miss)
    // Both compute and write -> last write wins, but both might read corrupt data
    if (resultCache.find(key.str()) != resultCache.end()) {
        return resultCache[key.str()];
    }
    
    int result = a * b;
    resultCache[key.str()] = result;  // RACE: Unprotected write
    return result;
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
