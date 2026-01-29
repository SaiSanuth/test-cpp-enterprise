#include <gtest/gtest.h>
#include "Calculator.h"
#include "StringHelper.h"
#include "Logger.h"
#include <sstream>

using namespace enterprise;

// ENTERPRISE INTEGRATION TESTS - DETERMINISTIC FAILURES
// ======================================================
// These tests expose realistic enterprise bugs that:
// ✓ Always fail when bug is present (no race conditions)
// ✓ Pass compilation
// ✓ Unit tests miss them
// ✗ Integration tests catch them

TEST(IntegrationTest, CalculatorNegativeNumberHandling) {
    // ENTERPRISE SCENARIO: Financial system with debits and credits
    // BUG: multiply() returns abs() for negative inputs
    // MISSED BY: Unit tests that only test positive numbers
    
    Calculator calc;
    int result = calc.multiply(-5, 6);
    
    // WILL FAIL: Returns 30 instead of -30
    EXPECT_EQ(result, -30) 
        << "\\n=== BUG: API CONTRACT VIOLATION ===\\n"
        << "Function: Calculator::multiply()\\n"
        << "Expected: -30, Got: " << result << "\\n"
        << "Issue: Returns abs(a*b) for negative inputs\\n"
        << "File: src/Calculator.cpp\\n"
        << "Impact: Financial calculations corrupted\\n";
}

TEST(IntegrationTest, CalculatorMixedSignOperations) {
    Calculator calc;
    
    // All WILL FAIL with current bug
    EXPECT_EQ(calc.multiply(-3, -4), 12);
    EXPECT_EQ(calc.multiply(3, -4), -12);
    EXPECT_EQ(calc.multiply(-3, 4), -12);
}

TEST(IntegrationTest, CrossModuleWorkflow) {
    // ENTERPRISE: Real financial calculation workflow
    Calculator calc;
    Logger& logger = Logger::getInstance();
    
    int revenue = 1000;
    int expenses = -300;
    int profit = calc.add(revenue, expenses);
    int taxLiability = calc.multiply(profit, -1);
    
    // Basic test passes
    EXPECT_EQ(taxLiability, -700)
        << "\nCross-module bug in financial workflow\n"
        << "Expected tax: -700, Got: " << taxLiability << "\n";
    
    logger.info("Tax calculated: " + std::to_string(taxLiability));
}

TEST(IntegrationTest, CacheInterdependencyBug) {
    // SUBTLE BUG: Operations depend on each other when cache enabled
    // Unit tests don't catch this because they test methods in isolation
    Calculator calc;
    
    // Enable caching for "performance"
    calc.enableCache(true);
    
    // Scenario 1: Add operation sets lastResult
    int sum = calc.add(10, 5);  // lastResult = 15
    EXPECT_EQ(sum, 15);
    EXPECT_EQ(calc.getLastResult(), 15);
    
    // BUG: factorial() uses lastResult as starting point!
    // factorial(3) should be 1*2*3 = 6
    // But with bug: starts from 15, then *2*3 = 90
    int fact = calc.factorial(3);
    EXPECT_EQ(fact, 6)
        << "\n=== INTERDEPENDENCY BUG DETECTED ===\n"
        << "factorial(3) expected: 6\n"
        << "factorial(3) got: " << fact << "\n"
        << "Root cause: factorial() incorrectly uses lastResult from previous add()\n"
        << "File: src/Calculator.cpp line ~78\n"
        << "Bug: int result = cacheEnabled ? lastResult : 1;\n"
        << "Impact: Chained calculations produce wrong results\n"
        << "Fix: Always initialize result = 1, regardless of cache state\n";
}

TEST(IntegrationTest, MultipleOperationsWithCache) {
    // COMPLEX: Multiple operations show cascading failures
    Calculator calc;
    calc.enableCache(true);
    
    // Operation sequence matters!
    int a = calc.add(100, 50);       // lastResult = 150
    int b = calc.subtract(200, 100); // lastResult = 100  
    int c = calc.multiply(5, 5);     // lastResult = 25
    int d = calc.factorial(4);       // BUG: starts from 25, not 1!
    
    // factorial(4) should be 24, not 25*2*3*4 = 600
    EXPECT_EQ(d, 24)
        << "\n=== CASCADING FAILURE ===\n"
        << "After multiple cached operations, factorial gives wrong result\n"
        << "Expected: 24, Got: " << d << "\n";
}
