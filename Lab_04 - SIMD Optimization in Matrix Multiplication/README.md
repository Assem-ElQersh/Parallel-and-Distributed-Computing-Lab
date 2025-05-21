# Lab 04: SIMD Optimization in Matrix Multiplication

## Overview
This lab implements and compares different approaches to matrix multiplication, focusing on the impact of memory access patterns and cache locality on performance. The implementation demonstrates how simple optimizations like matrix transposition can dramatically improve performance.

## Implementation Details
- Language: C++
- Matrix Size: 2048×2048
- Key Components:
  - Standard matrix multiplication
  - Matrix multiplication with transposition
  - Performance measurement using high-resolution clock
  - Random matrix initialization
  - Result verification

## Approaches Implemented

### 1. Standard Algorithm
```cpp
C[i,j] = Σ A[i,k] × B[k,j]
```
- Accesses matrix B in column-major order
- Poor cache locality
- Higher number of cache misses

### 2. Algorithm with Transposition
```cpp
C[i,j] = Σ A[i,k] × B_T[j,k]
```
- Transposes matrix B before multiplication
- Ensures sequential memory access for both matrices
- Better cache locality
- Additional O(N²) cost for transposition

## Results and Analysis

### Performance Comparison
- **Method 1 (Standard)**: 493.656 seconds
- **Method 2 (With transposition)**: 20.7779 seconds
- **Speedup factor**: 23.76x
- **Transposition overhead**: 0.0623 seconds

### Key Findings
1. **Cache Locality Impact**:
   - Method 1: A accessed sequentially, B accessed with stride N
   - Method 2: Both A and B_T accessed sequentially
   - Sequential access patterns significantly improve cache utilization

2. **Memory Access Patterns**:
   - Column-major access causes frequent cache misses
   - Row-major access maximizes cache line utilization
   - Memory access patterns can be more important than computational complexity

3. **Overhead vs. Benefit**:
   - Transposition overhead is negligible (0.0623s)
   - Significant time saved during multiplication (472.88s)
   - Preprocessing for better memory access is highly beneficial

## How to Run
1. Compile the code:
   ```bash
   g++ -O3 code.cpp -o matrix_mult
   ```
2. Run the executable:
   ```bash
   ./matrix_mult
   ```

## Requirements
- C++ compiler with C++11 support
- Sufficient memory for 2048×2048 matrices
- Modern CPU with cache hierarchy

## Note on AVX Support
While this implementation doesn't use AVX instructions, they could provide additional speedup:
- Compile with AVX support: `g++ -mavx -mavx2 -mfma code.cpp`
- Would enable processing multiple elements in parallel
- Current implementation is limited to scalar operations 