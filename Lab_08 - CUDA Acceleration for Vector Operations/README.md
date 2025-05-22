# Lab 8: CUDA Acceleration for Vector Operations

## Problem Statement
Implement and analyze CUDA-accelerated vector operations, focusing on:
1. Vector addition with large datasets (2^24 elements)
2. Performance comparison between CUDA and CPU implementations
3. Analysis of GPU acceleration benefits and overhead

## Implementation Details

### Vector Addition Implementation
- Vector size: 2^24 elements (16,777,216)
- Two implementations:
  1. CUDA GPU version
  2. Sequential CPU version (C++)

### Key Components
1. CUDA Implementation
   - Uses pinned memory for better transfer performance
   - Block size: 256 threads
   - Grid size: (N + blockSize - 1) / blockSize
   - Kernel function for vector addition
   - CUDA event timing for accurate performance measurement

2. CPU Implementation
   - Standard C++ vector operations
   - Sequential execution
   - High-resolution clock timing

3. Memory Management
   - GPU: Proper allocation and deallocation of device memory
   - CPU: Standard vector allocation
   - Pinned memory for host arrays to improve transfer speed

## Performance Analysis

### Test Configuration
- Vector size: 2^24 elements
- GPU: Tesla T4
- CUDA Version: 12.5

### Results
| Implementation | Execution Time (ms) | Speedup |
|----------------|-------------------|---------|
| CPU (C++)      | 19.00            | 1.0x    |
| GPU (CUDA)     | 24.93            | 0.76x   |

### Analysis
1. Performance Characteristics
   - GPU implementation (24.93ms) is slower than CPU (19ms) by approximately 24%
   - Primary factors affecting performance:
     * Memory transfer overhead between CPU and GPU
     * Simple nature of vector addition (one addition per element)
     * Insufficient computational intensity to overcome transfer overhead
   - CPU implementation advantages:
     * No memory transfer overhead
     * Efficient cache utilization
     * Modern CPU optimizations

2. Memory Transfer Impact
   - Host to Device transfer time
   - Device to Host transfer time
   - Pinned memory (cudaMallocHost) benefits:
     * Faster transfer speeds
     * Still incurs significant overhead for simple operations

3. Scalability and Performance Factors
   - Memory Access Patterns:
     * Contiguous memory access in both implementations
     * CUDA benefits from coalesced memory access
   - Hardware Utilization:
     * Tesla T4 GPU compute capabilities
     * CPU cache and optimization benefits
   - Break-even considerations:
     * Problem size
     * Computational intensity
     * Memory transfer requirements

## Conclusions
1. Performance Findings
   - CPU outperforms GPU for simple vector addition
   - Memory transfer overhead is the primary bottleneck
   - Pinned memory helps but doesn't overcome the overhead
   - Break-even point depends on computational intensity

2. Recommendations
   - Use CPU for simple vector operations
   - Consider GPU for more computationally intensive operations
   - Implement proper memory management strategies
   - Evaluate operation complexity before choosing implementation

3. Future Improvements
   - Implement stream-based asynchronous operations
   - Explore shared memory optimizations
   - Test with different vector sizes
   - Implement more complex vector operations
   - Consider hybrid CPU/GPU approaches

## How to Run
1. Compile the CUDA code:
   ```bash
   nvcc -o vector_add vector_add.cu
   ```

2. Compile the C++ code:
   ```bash
   g++ -o vector_add_cpu vector_add.cpp
   ```

3. Run the programs:
   ```bash
   ./vector_add      # GPU version
   ./vector_add_cpu  # CPU version
   ```

## Requirements
- NVIDIA GPU with CUDA support
- CUDA Toolkit 12.5 or later
- C++ compiler
- Minimum 4GB GPU memory recommended

## Notes
- Results may vary based on hardware configuration
- Vector size can be adjusted by modifying the N constant
- Block size can be tuned for optimal performance
- Memory transfer overhead should be considered in performance analysis 
