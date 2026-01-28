#include "Calculator.h"
#include "StringHelper.h"
#include "Logger.h"
#include <iostream>
#include <vector>

int main() {
    enterprise::Logger& logger = enterprise::Logger::getInstance();
    logger.info("Enterprise Application Starting...");
    
    // Calculator demo
    enterprise::Calculator calc;
    logger.info("Calculator: 10 + 5 = " + std::to_string(calc.add(10, 5)));
    logger.info("Calculator: 10 - 5 = " + std::to_string(calc.subtract(10, 5)));
    logger.info("Calculator: 10 * 5 = " + std::to_string(calc.multiply(10, 5)));
    logger.info("Calculator: 10 / 5 = " + std::to_string(calc.divide(10, 5)));
    logger.info("Calculator: Is 17 prime? " + std::string(calc.isPrime(17) ? "Yes" : "No"));
    logger.info("Calculator: 5! = " + std::to_string(calc.factorial(5)));
    
    // StringHelper demo
    std::string text = "Hello World";
    logger.info("StringHelper: Upper = " + enterprise::StringHelper::toUpper(text));
    logger.info("StringHelper: Lower = " + enterprise::StringHelper::toLower(text));
    logger.info("StringHelper: Reverse = " + enterprise::StringHelper::reverse(text));
    
    std::vector<std::string> words = enterprise::StringHelper::split(text, ' ');
    logger.info("StringHelper: Split words = " + std::to_string(words.size()));
    
    logger.info("Enterprise Application Completed Successfully!");
    return 0;
}
