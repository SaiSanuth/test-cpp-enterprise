cmake_minimum_required(VERSION 3.15)
project(EnterpriseApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(BUILD_TESTS "Build unit tests" ON)
option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(ENABLE_SANITIZERS "Enable address/memory sanitizers" OFF)

# Compiler flags
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Find dependencies
find_package(Threads REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Source files
set(SOURCES
    src/main.cpp
    src/core/Application.cpp
    src/core/Logger.cpp
    src/core/ConfigManager.cpp
    src/database/ConnectionPool.cpp
    src/database/QueryExecutor.cpp
    src/database/Transaction.cpp
    src/network/HttpClient.cpp
    src/network/TcpServer.cpp
    src/network/WebSocketHandler.cpp
    src/cache/LRUCache.cpp
    src/cache/DistributedCache.cpp
    src/auth/JWTManager.cpp
    src/auth/PasswordHasher.cpp
    src/auth/SessionManager.cpp
    src/utils/StringUtils.cpp
    src/utils/DateTimeUtils.cpp
    src/utils/CryptoUtils.cpp
    src/business/OrderProcessor.cpp
    src/business/PaymentGateway.cpp
    src/business/InventoryManager.cpp
)

# Create executable
add_executable(enterprise_app ${SOURCES})
target_link_libraries(enterprise_app PRIVATE Threads::Threads)

# Tests
if(BUILD_TESTS)
    enable_testing()
    
    # Download and setup Google Test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
    
    # Test files
    set(TEST_SOURCES
        tests/test_logger.cpp
        tests/test_connection_pool.cpp
        tests/test_lru_cache.cpp
        tests/test_jwt_manager.cpp
        tests/test_string_utils.cpp
        tests/test_order_processor.cpp
    )
    
    add_executable(unit_tests ${TEST_SOURCES}
        src/core/Logger.cpp
        src/database/ConnectionPool.cpp
        src/cache/LRUCache.cpp
        src/auth/JWTManager.cpp
        src/utils/StringUtils.cpp
        src/business/OrderProcessor.cpp
        src/utils/DateTimeUtils.cpp
        src/utils/CryptoUtils.cpp
    )
    
    target_link_libraries(unit_tests PRIVATE 
        gtest_main
        gmock_main
        Threads::Threads
    )
    
    include(GoogleTest)
    gtest_discover_tests(unit_tests)
endif()

# Installation
install(TARGETS enterprise_app DESTINATION bin)
