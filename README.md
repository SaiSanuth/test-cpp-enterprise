# Enterprise C++ Application - Test Repository

A complex, enterprise-grade C++ application designed for testing build triage AI systems.

## Features

- **Multi-layered Architecture**: Core, Database, Network, Cache, Auth, Business Logic
- **20+ Source Files**: Realistic enterprise codebase structure
- **Complex Dependencies**: Inter-module dependencies mimicking real systems
- **Unit Tests**: Google Test framework with comprehensive coverage
- **CI/CD Pipeline**: GitHub Actions with multiple build configurations
- **Intentional Bugs**: Various failure scenarios for AI testing

## Architecture

```
enterprise_app/
├── src/
│   ├── core/           # Application core (Logger, Config)
│   ├── database/       # Database layer (Connection pooling, Queries)
│   ├── network/        # Networking (HTTP, TCP, WebSocket)
│   ├── cache/          # Caching layer (LRU, Distributed)
│   ├── auth/           # Authentication (JWT, Password hashing)
│   ├── business/       # Business logic (Orders, Payments, Inventory)
│   └── utils/          # Utilities (String, DateTime, Crypto)
├── include/            # Header files
├── tests/              # Unit tests
└── .github/workflows/  # CI/CD pipelines
```

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Running Tests

```bash
cd build
ctest --output-on-failure
```

## CI/CD

GitHub Actions runs on every push:
- Debug and Release builds
- Unit tests
- Code coverage
- Static analysis
- Multiple compilers (GCC, Clang, MSVC)

## Test Scenarios

This repository includes various failure scenarios:
1. **CODE**: Null pointer dereferences, logic errors
2. **INFRA**: Build timeouts, missing dependencies
3. **CACHE**: Corrupted artifacts, dependency conflicts
