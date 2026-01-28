#include "Application.h"
#include <iostream>

int main(int argc, char** argv) {
    try {
        enterprise::Application app(argc, argv);
        
        if (!app.initialize()) {
            std::cerr << "Failed to initialize application" << std::endl;
            return 1;
        }
        
        return app.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
