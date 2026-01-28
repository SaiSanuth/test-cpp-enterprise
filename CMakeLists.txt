cmake_minimum_required(VERSION 3.15)
project(enterprise_app VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

# Find dependencies
find_package(Threads REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Calculator.cpp
    src/StringHelper.cpp
    src/Logger.cpp
)

# Create executable
add_executable(enterprise_app ${SOURCES})
target_link_libraries(enterprise_app PRIVATE Threads::Threads)

# Tests
option(BUILD_TESTS "Build tests" ON)

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
        tests/test_calculator.cpp
        tests/test_string_helper.cpp
        src/Calculator.cpp
        src/StringHelper.cpp
        src/Logger.cpp
    )
    
    add_executable(unit_tests ${TEST_SOURCES})
    target_link_libraries(unit_tests PRIVATE GTest::gtest_main Threads::Threads)
    
    include(GoogleTest)
    gtest_discover_tests(unit_tests)
endif()
