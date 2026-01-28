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
    
    // WILL FAIL: Gets 700 instead of -700
    EXPECT_EQ(taxLiability, -700)
        << "\\nCross-module bug in financial workflow\\n"
        << "Expected tax: -700, Got: " << taxLiability << "\\n";
    
    logger.info("Tax calculated: " + std::to_string(taxLiability));
}
