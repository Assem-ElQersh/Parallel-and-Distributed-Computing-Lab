# Lab 06: Dynamic Workload Distribution for Compute-Intensive Functions

## Overview
This lab implements and compares static and dynamic block-cyclic distribution strategies for parallelizing the Riemann Zeta function computation. The implementation demonstrates how different workload distribution methods affect performance in compute-intensive operations with irregular workloads.

## Implementation Details
- Language: C++
- Problem Size: n = 2048
- Number of Threads: 8
- Chunk Sizes Tested: 1, 2, 4, 8
- Key Components:
  - Sequential implementation
  - Static block-cyclic distribution
  - Dynamic block-cyclic distribution
  - Result verification

## Riemann Zeta Function
The implemented function is:
```
ζ(s,k) = 2^s × Σ₁ᵏ Σ₁ᵏ ((−1)^(i+1)) / ((i+j)^s)
```
where:
- s = 2 (in this implementation)
- k ranges from 1 to n
- i and j are summation indices

## Distribution Strategies

### 1. Static Block-Cyclic Distribution
- Workload divided into fixed-size chunks
- Chunks pre-assigned to threads
- Each thread processes its assigned chunks in order
- No runtime workload adjustment

### 2. Dynamic Block-Cyclic Distribution
- Workload divided into chunks
- Chunks stored in a shared queue
- Threads dynamically fetch chunks as they become available
- Better load balancing for irregular workloads
- Uses mutex for thread-safe queue access

## Results and Analysis

### Performance Comparison
| Distribution | Chunk 1 | Chunk 2 | Chunk 4 | Chunk 8 |
|-------------|---------|---------|---------|---------|
| Sequential  | 38.025s |         |         |         |
| Static      | 5.286s  | 5.267s  | 5.297s  | 5.433s  |
| Dynamic     | 5.174s  | 5.262s  | 5.239s  | 5.362s  |

### Key Findings
1. **Overall Performance**:
   - Both methods achieved ~7× speedup over sequential execution
   - Dynamic distribution slightly outperformed static distribution
   - Smaller chunk sizes (1 and 2) performed better than larger ones

2. **Best Performance**:
   - Dynamic distribution with chunk size 1
   - Execution time: 5.174s
   - Speedup: 7.35× over sequential execution

3. **Load Balancing**:
   - Dynamic distribution better handles load imbalance
   - O(k²) complexity causes varying computation times
   - Adaptive work assignment improves resource utilization

## How to Run
1. Compile the code:
   ```bash
   g++ -std=c++11 -O3 code.cpp -o riemann_zeta
   ```
2. Run the executable:
   ```bash
   ./riemann_zeta
   ```

## Requirements
- C++ compiler with C++11 support
- Multi-core CPU
- Sufficient memory for computation

## Implementation Details

### Static Distribution
```cpp
void static_block_cyclic(std::vector<double>& X, int chunk_size, int num_threads)
```
- Pre-assigns chunks to threads
- Each thread processes its chunks sequentially
- No runtime coordination needed

### Dynamic Distribution
```cpp
void dynamic_block_cyclic(std::vector<double>& X, int chunk_size, int num_threads)
```
- Uses a shared queue for chunk distribution
- Threads fetch chunks dynamically
- Mutex ensures thread-safe queue access
- Better load balancing for irregular workloads

## Conclusion
- Dynamic block-cyclic distribution provides better performance
- Smaller chunk sizes are more effective
- Load balancing is crucial for irregular workloads
- Synchronization overhead is outweighed by better resource utilization
- The O(k²) complexity of the Riemann Zeta function makes dynamic distribution particularly effective 