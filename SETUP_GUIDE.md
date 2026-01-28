# Enterprise C++ Application - Setup Guide

## Prerequisites

- **CMake** 3.15 or higher
- **C++17** compatible compiler:
  - GCC 8+ (Linux)
  - Clang 7+ (macOS)
  - MSVC 2019+ (Windows)
- **Git**
- **GitHub Account**

## Quick Start

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/test-cpp-enterprise.git
cd test-cpp-enterprise
```

### 2. Build the Project

**Linux/macOS:**
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

**Windows (Visual Studio):**
```powershell
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

### 3. Run Tests

```bash
cd build
ctest --output-on-failure
```

## Project Structure

```
test-cpp-enterprise/
├── .github/workflows/     # CI/CD pipelines
├── src/                   # Source files
│   ├── core/             # Application core
│   ├── database/         # Database layer
│   ├── cache/            # Caching layer
│   ├── utils/            # Utilities
│   └── stubs.cpp         # Stub implementations
├── include/              # Header files
├── tests/                # Unit tests
├── CMakeLists.txt        # Build configuration
└── README.md
```

## GitHub Actions CI/CD

The project includes two workflows:

### 1. Main CI Pipeline (`ci.yml`)
- Builds on Linux, Windows, macOS
- Runs unit tests
- Performs static analysis
- Generates code coverage

### 2. Failure Scenarios (`failure-scenarios.yml`)
- Simulates different failure types
- Used for testing build triage AI
- Can be triggered manually

## Triggering Builds

### Push to main branch:
```bash
git push origin main
```

### Trigger failure scenario:
1. Go to Actions tab on GitHub
2. Select "Failure Scenarios" workflow
3. Click "Run workflow"
4. Choose failure type

## Testing the Triage System

1. **Create intentional bug**: Edit a source file
2. **Commit and push**: Trigger CI pipeline
3. **Wait for failure**: Check Actions tab
4. **Run triage**: Use `github_triage.py` script
5. **Review analysis**: Check AI categorization

## Common Build Issues

| Error | Cause | Fix |
|-------|-------|-----|
| CMake not found | Missing CMake | Install CMake 3.15+ |
| Compiler not found | Missing toolchain | Install GCC/Clang/MSVC |
| Google Test fails | Network issue | Retry build |
| Tests timeout | Resource limits | Reduce test concurrency |

## Next Steps

1. **Push to GitHub**: Create repository and push code
2. **Enable Actions**: Ensure GitHub Actions are enabled
3. **Configure Secrets**: Add any required secrets
4. **Test CI**: Make a test commit
5. **Integrate Triage**: Connect AI triage system

## Support

For issues or questions, check:
- GitHub Issues
- CI/CD logs in Actions tab
- Build output in artifacts
