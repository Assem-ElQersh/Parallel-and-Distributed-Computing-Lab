# Parallel and Distributed Computing Labs

This repository contains a series of laboratory assignments focused on parallel and distributed computing concepts, from basic speedup analysis to advanced GPU programming.

## Overview

The labs progress from theoretical concepts to practical implementations, covering various parallel computing technologies and optimization techniques. Each lab focuses on specific aspects of parallel computing and performance optimization.

## Lab Assignments

### Lab 01: CCR Speedup Function
**Objective**: Implement and visualize the speedup function in parallel computing.

**Task**:
- Implement a Python program to plot a 3D graph of the speedup function:
  ```
  S_γ(2^q, 2^k) = (γ(2^k - 1)) / (2q + γ(2^(k-q) - 1 + q))
  ```
- Plot the surface for k=10
- Highlight the optimal speedup curve in red
- γ represents the compute-to-communication ratio

### Lab 02: Parameterized Speedup and Efficiency Functions
**Objective**: Analyze and visualize scaled speedup and efficiency functions.

**Tasks**:
1. Implement scaled speedup function:
   ```
   S_γ(p) = (f + (1-f)p^δ) / (f + (1-f)p^(δ-1))
   ```
2. Implement scaled efficiency function:
   ```
   E_γ(p) = S_γ(p)/p = (f + (1-f)p^δ) / (pf + (1-f)p^δ)
   ```
- Create separate plots for f=0.1 and f=0.5
- γ=p^δ represents parameterized compute-to-communication ratio
- p is the number of processors

### Lab 03: Cache Locality Optimization in Matrix Multiplication
**Objective**: Study the impact of cache locality on matrix multiplication performance.

**Tasks**:
- Implement matrix multiplication C = A × B for 2048×2048 matrices using:
  1. Standard algorithm: C[i,j] = Σ A[i,k] × B[k,j]
  2. Algorithm with transposition: C[i,j] = Σ A[i,k] × B_T[j,k]
- Compare execution times
- Analyze cache locality effects

### Lab 04: SIMD Optimization in Matrix Multiplication
**Objective**: Explore SIMD vectorization for matrix multiplication.

**Tasks**:
- Implement matrix multiplication for 2048×2048 matrices using:
  1. Algorithm with transposition
  2. Algorithm with transposition and AVX SIMD intrinsics
- Compare execution times
- Analyze SIMD vectorization impact

### Lab 05: Parallel Distribution Methods for Matrix Operations
**Objective**: Compare different parallel distribution strategies.

**Tasks**:
- Implement matrix subtraction C = A - B for 2048×2048 matrices using:
  1. Serial implementation
  2. Parallel implementation with Pthreads using:
     - Block distribution
     - Cyclic distribution
     - Block-cyclic distribution
- Use 8 threads
- Compare performance characteristics

### Lab 06: Dynamic Workload Distribution
**Objective**: Study dynamic workload distribution for irregular computations.

**Tasks**:
- Implement Riemann Zeta function calculation:
  ```
  ζ(s,k) = 2^s × Σ₁ᵏ Σ₁ᵏ ((−1)^(i+1)) / ((i+j)^s)
  ```
- Compare two parallelization strategies:
  1. Static block-cyclic distribution
  2. Dynamic block-cyclic distribution
- Test with n=2048, 8 threads
- Analyze chunk sizes: 1, 2, 4, and 8

### Lab 07: OpenMP Parallelization Strategies
**Objective**: Explore OpenMP parallelization for different algorithmic patterns.

**Tasks**:
1. Matrix multiplication (A×B=C) with N=M=L=256
2. Knapsack problem using dynamic programming:
   - N=C=1024
   - dp[i][w] = max(dp[i-1][w], dp[i-1][w-weight[i]]+value[i])
- Compare sequential and parallel implementations
- Analyze parallelization strategies

### Lab 08: CUDA Acceleration for Vector Operations
**Objective**: Compare CPU and GPU implementations of vector operations.

**Tasks**:
Implement in both CUDA and standard C++:
1. Vector addition: C = A + B for vectors of size 2^24
2. 4D vector normalization: V_normalized = V/||V|| for 2^22 vectors
   - ||V|| = √(V.x² + V.y² + V.z² + V.w²)
- Compare execution times
- Analyze performance differences

## Requirements

- Python 3.x (for Labs 01-02)
- C/C++ compiler with OpenMP support
- CUDA toolkit (for Lab 08)
- AVX SIMD support (for Lab 04)

## Note

This repository contains both problem statements and their solutions. The solutions are organized in separate directories for each lab. 