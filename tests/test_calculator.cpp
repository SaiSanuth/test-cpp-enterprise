#include <gtest/gtest.h>
#include "Calculator.h"

using namespace enterprise;

TEST(CalculatorTest, Addition) {
    Calculator calc;
    EXPECT_EQ(calc.add(2, 3), 5);
    EXPECT_EQ(calc.add(-1, 1), 0);
    EXPECT_EQ(calc.add(0, 0), 0);
}

TEST(CalculatorTest, Subtraction) {
    Calculator calc;
    EXPECT_EQ(calc.subtract(5, 3), 2);
    EXPECT_EQ(calc.subtract(0, 5), -5);
    EXPECT_EQ(calc.subtract(10, 10), 0);
}

TEST(CalculatorTest, Multiplication) {
    Calculator calc;
    EXPECT_EQ(calc.multiply(3, 4), 12);
    EXPECT_EQ(calc.multiply(0, 100), 0);
    EXPECT_EQ(calc.multiply(-2, 5), -10);
}

TEST(CalculatorTest, Division) {
    Calculator calc;
    EXPECT_DOUBLE_EQ(calc.divide(10, 2), 5.0);
    EXPECT_DOUBLE_EQ(calc.divide(7, 2), 3.5);
    EXPECT_THROW(calc.divide(10, 0), std::invalid_argument);
}

TEST(CalculatorTest, IsPrime) {
    Calculator calc;
    EXPECT_TRUE(calc.isPrime(2));
    EXPECT_TRUE(calc.isPrime(17));
    EXPECT_FALSE(calc.isPrime(1));
    EXPECT_FALSE(calc.isPrime(4));
    EXPECT_FALSE(calc.isPrime(0));
}

TEST(CalculatorTest, Factorial) {
    Calculator calc;
    EXPECT_EQ(calc.factorial(0), 1);
    EXPECT_EQ(calc.factorial(1), 1);
    EXPECT_EQ(calc.factorial(5), 120);
    EXPECT_THROW(calc.factorial(-1), std::invalid_argument);
}

TEST(CalculatorTest, Power) {
    Calculator calc;
    EXPECT_DOUBLE_EQ(calc.power(2, 3), 8.0);
    EXPECT_DOUBLE_EQ(calc.power(5, 0), 1.0);
    EXPECT_DOUBLE_EQ(calc.power(10, 2), 100.0);
}
