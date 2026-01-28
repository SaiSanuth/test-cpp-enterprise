# Test Failure Scenarios

This document describes the intentional bugs and failure scenarios included for testing the triage AI.

## Scenario 1: NULL Pointer Dereference (CODE)

**File**: Will be created in `src/bug.cpp` by workflow  
**Type**: Code error  
**Trigger**: Push to `test-failures` branch or manual workflow

```cpp
int* ptr = nullptr;
*ptr = 42;  // CRASH!
```

**Expected AI Analysis**:
- Category: CODE
- Confidence: High
- Root cause: NULL pointer dereference
- Suspect commits: Recent changes to pointer logic

## Scenario 2: Build Timeout (INFRA)

**Type**: Infrastructure issue  
**Trigger**: Manual workflow with `infra_timeout` input

**Symptoms**:
- Build exceeds timeout limit
- Process killed by runner
- No code compilation errors

**Expected AI Analysis**:
- Category: INFRA
- Confidence: High
- Root cause: Resource exhaustion or timeout
- Recommendation: Increase timeout or optimize build

## Scenario 3: Cache Corruption (CACHE)

**Type**: Cache/dependency issue  
**Trigger**: Manual workflow with `cache_corruption` input

**Symptoms**:
- CMake configuration fails
- Corrupted state in build directory
- Works after clean build

**Expected AI Analysis**:
- Category: CACHE
- Confidence: Medium-High
- Root cause: Corrupted build artifacts
- Recommendation: Clear cache and rebuild

## Scenario 4: Missing Dependency (INFRA)

**Type**: Infrastructure/dependency issue  
**Trigger**: Manual workflow with `dependency_missing` input

**Symptoms**:
- Command not found errors
- Missing tool/library errors
- Environment setup failure

**Expected AI Analysis**:
- Category: INFRA
- Confidence: High
- Root cause: Missing build dependency
- Recommendation: Install required tools

## Additional Manual Scenarios

### Compile Error
Edit any `.cpp` file:
```cpp
int calculate() {
    return "string";  // Type mismatch
}
```

### Linker Error
Remove function implementation but keep declaration

### Test Failure
Edit test to expect wrong value:
```cpp
EXPECT_EQ(2 + 2, 5);  // Wrong expectation
```

## Testing Strategy

1. **Baseline**: Ensure main branch builds successfully
2. **Introduce Bug**: Apply one scenario
3. **Trigger CI**: Push or run workflow
4. **Collect Logs**: Download failure artifacts
5. **Run Triage**: Analyze with AI system
6. **Validate**: Check AI categorization accuracy

## Metrics to Track

- **Accuracy**: % of correct categorizations
- **Precision**: % of CODE flags that are actually code issues
- **Recall**: % of code issues correctly identified
- **Response Time**: Time from failure to analysis
- **False Positives**: INFRA issues flagged as CODE

## Expected Results

| Scenario | Expected Category | Expected Confidence |
|----------|-------------------|---------------------|
| NULL pointer | CODE | 90-100% |
| Timeout | INFRA | 80-95% |
| Cache corrupt | CACHE | 70-85% |
| Missing dep | INFRA | 85-95% |
| Compile error | CODE | 90-100% |
| Linker error | CODE/INFRA | 70-85% |
| Test failure | CODE | 85-95% |
