# Lab 05: Parallel Distribution Methods for Matrix Operations

## Overview
This lab implements and compares different parallel distribution strategies for matrix subtraction operations using Pthreads. The implementation demonstrates how different workload distribution methods affect performance in memory-bound operations.

## Implementation Details
- Language: C++
- Matrix Size: 2048×2048
- Number of Threads: 8
- Block Size: 64
- Key Components:
  - Serial implementation
  - Block distribution
  - Cyclic distribution
  - Block-cyclic distribution
  - Result verification

## Distribution Strategies

### 1. Serial Implementation
- Simple sequential matrix subtraction
- Baseline for performance comparison
- No parallelization overhead

### 2. Block Distribution
- Each thread processes a continuous chunk of rows
- Workload divided into equal-sized blocks
- Extra rows distributed to first few threads if N % NUM_THREADS ≠ 0
- Good for cache locality within blocks

### 3. Cyclic Distribution
- Threads process rows in a round-robin fashion
- Each thread handles rows: i, i + NUM_THREADS, i + 2*NUM_THREADS, ...
- Better memory access patterns
- Improved CPU prefetching

### 4. Block-Cyclic Distribution
- Combines aspects of both block and cyclic distribution
- Matrix divided into blocks of size BLOCK_SIZE
- Blocks assigned to threads in a cyclic manner
- Potential for better load balancing

## Results and Analysis

### Performance Comparison
- **Serial**: 0.0188s
- **Block Distribution**: 0.0188s
- **Cyclic Distribution**: 0.0173s (Fastest)
- **Block-Cyclic**: 0.0230s (Slowest)

### Key Findings
1. **Memory-Bound Nature**:
   - Performance limited by memory bandwidth
   - Thread overhead can outweigh parallel benefits
   - Memory access patterns crucial for performance

2. **Cyclic Distribution Advantages**:
   - Better memory utilization through strided access
   - Improved CPU prefetching
   - Reduced cache conflicts
   - Equal workload distribution
   - Non-overlapping memory access

3. **Block-Cyclic Challenges**:
   - Higher thread coordination overhead
   - Suboptimal block size
   - Frequent thread synchronization

## How to Run
1. Compile the code:
   ```bash
   g++ -std=c++11 -O3 code.cpp -o matrix_subtraction
   ```
2. Run the executable:
   ```bash
   ./matrix_subtraction
   ```

## Requirements
- C++ compiler with C++11 support
- Pthreads library
- Sufficient memory for 2048×2048 matrices
- Multi-core CPU

## Optimization Suggestions
1. **Workload Optimization**:
   - Increase matrix size to reduce thread overhead impact
   - Optimize block size to match cache sizes
   - Use thread pools to reduce creation overhead

2. **Technical Improvements**:
   - Implement SIMD instructions (AVX) for faster operations
   - Align memory access with cache lines
   - Consider NUMA-aware allocation for multi-socket systems

## Conclusion
- Matrix subtraction is primarily memory-bound
- Parallelism benefits depend heavily on memory access patterns
- Cyclic distribution provides best performance for this workload
- Block-based approaches would be more effective for compute-intensive tasks 