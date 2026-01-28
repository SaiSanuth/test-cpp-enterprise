#pragma once
#include <memory>
#include <string>

namespace enterprise {
namespace core {
    class Logger;
    class ConfigManager;
}
namespace database {
    class ConnectionPool;
}
namespace network {
    class TcpServer;
}
namespace cache {
    class DistributedCache;
}

class Application {
public:
    Application(int argc, char** argv);
    ~Application();
    
    bool initialize();
    int run();
    void shutdown();
    
    // Getters
    core::Logger& getLogger();
    core::ConfigManager& getConfig();
    
private:
    bool parseCommandLine(int argc, char** argv);
    bool loadConfiguration();
    bool initializeSubsystems();
    
    // Raw pointers for stub implementation (avoids incomplete type issues)
    database::ConnectionPool* dbPool_;
    network::TcpServer* server_;
    cache::DistributedCache* cache_;
    
    bool initialized_;
    bool running_;
};

} // namespace enterprise
