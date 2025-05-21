#include <iostream>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

int main() {
    const int N = 2048; // Matrix size
    
    // Allocate memory dynamically for matrices
    float* A = new float[N*N]; // Matrix A (NxN)
    float* B = new float[N*N]; // Matrix B (NxN)
    float* C = new float[N*N]; // Result matrix (NxN)
    float* Bt = new float[N*N]; // Transposed B matrix (NxN)

    // Initialize A and B with random values
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int i = 0; i < N*N; ++i) {
        A[i] = dis(gen);
        B[i] = dis(gen);
    }

    // Case 1: Multiply without transposing B
    auto start1 = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < N; ++k) {
                sum += A[i*N + k] * B[k*N + j]; // Standard row-column multiplication
            }
            C[i*N + j] = sum;
        }
    }
    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(stop1 - start1);
    cout << "Time without transpose: " << duration1.count() << " ms" << endl;

    // Case 2: Transpose B and then multiply
    auto start2 = high_resolution_clock::now();
    
    // Transpose B into Bt (swap rows and columns)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            Bt[i*N + j] = B[j*N + i]; // Transposition operation
        }
    }

    // Multiply A and transposed B (Bt)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < N; ++k) {
                sum += A[i*N + k] * Bt[j*N + k]; // Better cache locality
            }
            C[i*N + j] = sum;
        }
    }
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(stop2 - start2);
    cout << "Time with transpose: " << duration2.count() << " ms" << endl;

    // Cleanup allocated memory
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] Bt;

    return 0;
}