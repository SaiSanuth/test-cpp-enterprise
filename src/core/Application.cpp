#include "Application.h"
#include "Logger.h"
#include "ConfigManager.h"
#include "ConnectionPool.h"
// #include "TcpServer.h"  // Stub - not needed for minimal build
#include "DistributedCache.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace enterprise {

Application::Application(int argc, char** argv) 
    : initialized_(false)
    , running_(false) {
    parseCommandLine(argc, argv);
}

Application::~Application() {
    shutdown();
}

bool Application::parseCommandLine(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "--config" && i + 1 < argc) {
            core::ConfigManager::getInstance().set("config_file", std::string(argv[++i]));
        } else if (arg == "--verbose") {
            core::Logger::getInstance().setLogLevel(core::LogLevel::DEBUG);
        } else if (arg == "--port" && i + 1 < argc) {
            core::ConfigManager::getInstance().set("port", std::stoi(argv[++i]));
        }
    }
    return true;
}

bool Application::initialize() {
    if (initialized_) {
        LOG_WARN("Application already initialized");
        return true;
    }
    
    LOG_INFO("Initializing Enterprise Application v1.0.0");
    
    if (!loadConfiguration()) {
        LOG_ERROR("Failed to load configuration");
        return false;
    }
    
    if (!initializeSubsystems()) {
        LOG_ERROR("Failed to initialize subsystems");
        return false;
    }
    
    initialized_ = true;
    LOG_INFO("Application initialized successfully");
    return true;
}

bool Application::loadConfiguration() {
    auto& config = core::ConfigManager::getInstance();
    
    // Load from environment first
    config.loadFromEnvironment();
    
    // Then override with file
    std::string configFile = config.getString("config_file", "config.ini");
    if (!config.loadFromFile(configFile)) {
        LOG_WARN("Could not load config file, using defaults");
    }
    
    // Set defaults
    if (!config.has("port")) config.set("port", 8080);
    if (!config.has("max_connections")) config.set("max_connections", 100);
    
    return true;
}

bool Application::initializeSubsystems() {
    auto& config = core::ConfigManager::getInstance();
    
    // Initialize database connection pool
    std::string dbUrl = config.getString("DATABASE_URL", "localhost:5432");
    int maxConn = config.getInt("max_connections", 20);
    
    try {
        dbPool_ = std::make_unique<database::ConnectionPool>(dbUrl, 5, maxConn);
        LOG_INFO("Database connection pool initialized");
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to initialize database: " + std::string(e.what()));
        return false;
    }
    
    // Initialize distributed cache
    std::vector<cache::CacheNode> cacheNodes = {
        {"localhost", 6379, true, std::chrono::system_clock::now()},
        {"localhost", 6380, true, std::chrono::system_clock::now()}
    };
    
    try {
        cache_ = std::make_unique<cache::DistributedCache>(cacheNodes, 10000);
        LOG_INFO("Distributed cache initialized");
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to initialize cache: " + std::string(e.what()));
        return false;
    }
    
    // Initialize TCP server
    int port = config.getInt("port", 8080);
    try {
        server_ = std::make_unique<network::TcpServer>(port);
        LOG_INFO("TCP server initialized on port " + std::to_string(port));
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to initialize server: " + std::string(e.what()));
        return false;
    }
    
    return true;
}

int Application::run() {
    if (!initialized_) {
        LOG_ERROR("Application not initialized");
        return 1;
    }
    
    LOG_INFO("Starting application...");
    running_ = true;
    
    // Simulate application running
    LOG_INFO("Application running. Press Ctrl+C to stop.");
    
    // In real app, this would be event loop
    while (running_) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}

void Application::shutdown() {
    if (!initialized_) {
        return;
    }
    
    LOG_INFO("Shutting down application...");
    running_ = false;
    
    server_.reset();
    cache_.reset();
    dbPool_.reset();
    
    LOG_INFO("Application shut down complete");
    initialized_ = false;
}

core::Logger& Application::getLogger() {
    return core::Logger::getInstance();
}

core::ConfigManager& Application::getConfig() {
    return core::ConfigManager::getInstance();
}

} // namespace enterprise
