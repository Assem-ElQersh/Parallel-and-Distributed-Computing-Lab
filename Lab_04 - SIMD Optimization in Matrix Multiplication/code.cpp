#include <iostream>
#include <random>
#include <chrono>
// Removed immintrin.h since we're not using AVX instructions

// Define matrix size
const int N = 2048;

int main() {
    std::cout << "Matrix Multiplication Performance Comparison (N = " << N << ")\n";
    std::cout << "=======================================================\n\n";
    
    // Allocate memory for matrices
    float* A = new float[N * N];
    float* B = new float[N * N];
    float* C1 = new float[N * N]; // For normal method
    float* C2 = new float[N * N]; // For transposed method
    float* B_T = new float[N * N];

    if (!A || !B || !C1 || !C2 || !B_T) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return 1;
    }

    // Initialize random number generator
    std::default_random_engine gen(42); // Fixed seed for reproducibility
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // Fill A and B with random values
    std::cout << "Initializing matrices with random values...\n";
    for (int i = 0; i < N * N; i++) {
        A[i] = dist(gen);
        B[i] = dist(gen);
    }

    // Transpose B into B_T
    std::cout << "Transposing matrix B...\n";
    auto transpose_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B_T[j * N + i] = B[i * N + j];
        }
    }
    auto transpose_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> transpose_time = transpose_end - transpose_start;
    std::cout << "Transposition time: " << transpose_time.count() << " seconds\n\n";

    // METHOD 1: Standard matrix multiplication without transposition
    std::cout << "METHOD 1: Standard multiplication without transposing B\n";
    auto start1 = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0.0f;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j]; // Note: Accessing B in column-major order
            }
            C1[i * N + j] = sum;
        }
    }
    
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed1 = end1 - start1;
    std::cout << "Computation time: " << elapsed1.count() << " seconds\n";

    // Print first 3x3 of C1 (standard)
    std::cout << "First 3x3 elements of result matrix C1:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << C1[i * N + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    // METHOD 2: Matrix multiplication with transposed B
    std::cout << "METHOD 2: Matrix multiplication with transposed B\n";
    auto start2 = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0.0f;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B_T[j * N + k]; // Using transposed B for better cache performance
            }
            C2[i * N + j] = sum;
        }
    }
    
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed2 = end2 - start2;
    std::cout << "Computation time: " << elapsed2.count() << " seconds\n";

    // Print first 3x3 of C2 (with transposed B)
    std::cout << "First 3x3 elements of result matrix C2:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << C2[i * N + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    // Verify results match
    bool results_match = true;
    float max_diff = 0.0f;
    for (int i = 0; i < N * N && results_match; i++) {
        float diff = std::abs(C1[i] - C2[i]);
        max_diff = std::max(max_diff, diff);
        if (diff > 1e-3) {
            results_match = false;
        }
    }
    
    if (results_match) {
        std::cout << "Results match! Maximum difference: " << max_diff << "\n";
    } else {
        std::cout << "Results don't match! Maximum difference: " << max_diff << "\n";
    }

    // Performance summary
    std::cout << "\nPERFORMANCE SUMMARY:\n";
    std::cout << "Method 1 (Standard without transposition): " << elapsed1.count() << " seconds\n";
    std::cout << "Method 2 (With transposed B): " << elapsed2.count() << " seconds\n";
    
    if (elapsed1.count() > elapsed2.count()) {
        double speedup = elapsed1.count() / elapsed2.count();
        std::cout << "Speedup from transposition: " << speedup << "x\n\n";
    } else {
        double slowdown = elapsed2.count() / elapsed1.count();
        std::cout << "Slowdown from transposition: " << slowdown << "x\n\n";
    }

    // Free allocated memory
    delete[] A;
    delete[] B;
    delete[] C1;
    delete[] C2;
    delete[] B_T;

    return 0;
}