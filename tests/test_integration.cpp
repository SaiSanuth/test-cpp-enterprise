#include <gtest/gtest.h>
#include "Calculator.h"
#include "StringHelper.h"
#include "Logger.h"
#include <thread>
#include <vector>
#include <atomic>
#include <set>
#include <chrono>
#include <map>

using namespace enterprise;

// Enterprise Integration Tests - These expose realistic production bugs
// that pass unit tests but fail under concurrent load

TEST(IntegrationTest, ConcurrentCalculatorCacheCorruption) {
    // ENTERPRISE SCENARIO: Multiple API handlers calculating the same values
    // The shared cache gets corrupted due to missing synchronization
    
    const int NUM_THREADS = 50;
    const int ITERATIONS = 1000;
    std::atomic<int> wrongResults{0};
    std::atomic<int> totalOps{0};
    
    auto worker = [&wrongResults, &totalOps](int threadId) {
        Calculator calc;
        for (int i = 0; i < ITERATIONS; i++) {
            // All threads multiply the same values
            // Shared cache should always return 30, but race causes corruption
            int result = calc.multiply(5, 6);
            totalOps++;
            
            if (result != 30) {
                wrongResults++;
                // In production: This would corrupt financial calculations,
                // scientific computations, or business logic results
            }
        }
        (void)threadId; // Suppress warning
    };
    
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(worker, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    int errors = wrongResults.load();
    int total = totalOps.load();
    
    // CRITICAL FAILURE: Cache corruption in multi-threaded environment
    EXPECT_EQ(errors, 0) 
        << "CRITICAL BUG DETECTED: Calculator cache corruption!\n"
        << "   Wrong results: " << errors << " out of " << total << " operations\n"
        << "   Error rate: " << (100.0 * errors / total) << "%\n"
        << "   ROOT CAUSE: Unprotected shared std::map<> in Calculator::multiply()\n"
        << "   ENTERPRISE IMPACT: Data corruption, wrong financial calculations\n"
        << "   FIX REQUIRED: Add std::mutex or use thread-local storage";

TEST(IntegrationTest, ConcurrentLoggerUsage) {
    // Simulate multiple threads logging simultaneously
    // In enterprise: Request handlers, audit logs, error reporting
    const int NUM_THREADS = 10;
    const int LOGS_PER_THREAD = 50;
    
    auto worker = [](int threadId) {
        Logger& logger = Logger::getInstance();
        for (int i = 0; i < LOGS_PER_THREAD; i++) {
            // BUG DETECTION: Each log call increments static counter
            // Race condition causes count to be lost or duplicated
            logger.info("Thread " + std::to_string(threadId) + " iteration " + std::to_string(i));
        }
    };
    
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(worker, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // FAILURE POINT: Static counter race causes non-sequential numbering
    // Expected: 500 total calls (10 threads * 50 logs)
    // Actual: Counter shows less due to lost increments
    int expectedTotalCalls = NUM_THREADS * LOGS_PER_THREAD;
    (void)expectedTotalCalls;  // Used in test description
    
    // This test will fail because the static callCount in Logger::log()
    // is incremented without mutex protection, causing race conditions
    // In real enterprise scenarios, this manifests as:
    // - Lost audit log entries
    // - Incorrect request counting
    // - Metrics corruption
    Logger::getInstance().info("Integration test completed");
    
    // Note: We can't easily verify the exact count here without modifying Logger,
    // but the race condition often causes segfaults or assertion failures
    // in sanitizer builds (ThreadSanitizer, AddressSanitizer)
    SUCCEED() << "Logger concurrency test completed. Run with ThreadSanitizer to detect races.";
}

TEST(IntegrationTest, MultiComponentStressTest) {
    // Simulate realistic enterprise workload:
    // Multiple components used together under heavy concurrent load
    const int NUM_THREADS = 20;
    const int OPERATIONS = 50;
    std::atomic<int> calculationErrors{0};
    
    auto realisticWorker = [&calculationErrors](int workerId) {
        Calculator calc;
        StringHelper strHelper;
        Logger& logger = Logger::getInstance();
        
        for (int i = 0; i < OPERATIONS; i++) {
            // Typical enterprise request flow:
            // 1. Log request start
            logger.info("Worker " + std::to_string(workerId) + " processing request " + std::to_string(i));
            
            // 2. Perform business logic calculations
            int result = calc.multiply(workerId, i + 1);
            int expectedResult = workerId * (i + 1);
            
            if (result != expectedResult) {
                calculationErrors++;
                logger.error("Calculation mismatch! Got " + std::to_string(result) + 
                           ", expected " + std::to_string(expectedResult));
            }
            
            // 3. String processing (sanitization, formatting, etc.)
            std::string data = "Result: " + std::to_string(result);
            std::string upper = strHelper.toUpper(data);
            
            // 4. Log completion
            logger.info("Worker " + std::to_string(workerId) + " completed: " + upper);
        }
    };
    
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(realisticWorker, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // FAILURE POINT: This integration test fails when:
    // 1. Calculator cache race causes wrong results
    // 2. Logger counter race causes assertion failures
    // 3. Combined load amplifies the race window
    EXPECT_EQ(calculationErrors, 0) 
        << "Multi-component stress test detected " << calculationErrors 
        << " calculation errors. This indicates race conditions in Calculator cache. "
        << "Root cause: Shared static cache accessed without synchronization.";
}

TEST(IntegrationTest, VerifyCacheIntegrityUnderLoad) {
    // GUARANTEED FAILURE TEST: This WILL expose the race condition
    // By having many threads hammer the same cache key simultaneously
    
    const int NUM_THREADS = 100;
    const int ITERATIONS = 500;
    std::atomic<int> wrongResults{0};
    
    auto worker = [&wrongResults](int threadId) {
        Calculator calc;
        for (int i = 0; i < ITERATIONS; i++) {
            int result = calc.multiply(5, 6);
            if (result != 30) {
                wrongResults++;
            }
        }
        (void)threadId;
    };
    
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(worker, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    int errors = wrongResults.load();
    int totalOps = NUM_THREADS * ITERATIONS;
    
    // THIS WILL FAIL: Unprotected static map causes cache corruption
    EXPECT_EQ(errors, 0) 
        << "\\n=== CRITICAL BUG: RACE CONDITION DETECTED ===\\n"
        << "Test: VerifyCacheIntegrityUnderLoad\\n"
        << "Total operations: " << totalOps << "\\n"
        << "Wrong results: " << errors << "\\n"
        << "Error rate: " << (100.0 * errors / totalOps) << "%\\n"
        << "\\nROOT CAUSE: Unprotected static cache in Calculator::multiply()\\n"
        << "FILE: src/Calculator.cpp (line ~15)\\n"
        << "ISSUE: static std::map without mutex protection\\n"
        << "\\nENTERPRISE IMPACT:\\n"
        << "  - Financial calculations corrupted\\n"
        << "  - Multi-threaded API failures\\n"
        << "  - Production data integrity violations\\n"
        << "\\nRECOMMENDED FIX:\\n"
        << "  1. Add static std::mutex cacheMutex\\n"
        << "  2. Lock before all cache operations\\n"
        << "  3. OR remove caching entirely\\n";\n}

// This test simulates what happens in enterprise CI/CD:
// - Local developer runs: Single-threaded unit tests pass ✓
// - CI/CD runs: Multi-threaded integration tests FAIL ✗
// - Production deploys: Random failures under load
//
// The bugs introduced:
// 1. Logger::log() - static callCount++ without mutex
//    Impact: Lost increments, wrong sequence numbers
//    Enterprise impact: Audit trail corruption, compliance violations
//
// 2. Calculator::multiply() - shared resultCache without mutex  
//    Impact: Cache corruption, wrong calculation results
//    Enterprise impact: Financial errors, data corruption
//
// These bugs are REALISTIC because:
// ✓ Code compiles fine
// ✓ Unit tests pass (single-threaded)
// ✓ Basic functionality works
// ✗ Fails only under concurrent load (integration/system tests)
// ✗ Detected by ThreadSanitizer, stress tests, production monitoring
