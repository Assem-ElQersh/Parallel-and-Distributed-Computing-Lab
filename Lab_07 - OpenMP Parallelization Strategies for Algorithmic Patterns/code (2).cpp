#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Size of matrices
const int N = 256; // Matrix A: N x M
const int M = 256; // Matrix B: M x L
const int L = 256; // Matrix C: N x L

// Function to initialize a matrix with random values
void initializeMatrix(vector<vector<double>>& matrix, int rows, int cols) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(1.0, 10.0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = dis(gen);
        }
    }
}

// Sequential matrix multiplication
void sequentialMatrixMultiplication(const vector<vector<double>>& A,
                                  const vector<vector<double>>& B,
                                  vector<vector<double>>& C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < L; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < M; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Parallel matrix multiplication using OpenMP
void parallelMatrixMultiplication(const vector<vector<double>>& A,
                                const vector<vector<double>>& B,
                                vector<vector<double>>& C,
                                int num_threads) {
    // Set number of threads
    omp_set_num_threads(num_threads);
    
    // Parallelize the outer loop
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < L; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < M; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to check if both results are the same
bool verifyResults(const vector<vector<double>>& C1, const vector<vector<double>>& C2) {
    const double epsilon = 1e-6; // Tolerance for floating point comparison
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < L; j++) {
            if (abs(C1[i][j] - C2[i][j]) > epsilon) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    // Initialize matrices
    vector<vector<double>> A(N, vector<double>(M));
    vector<vector<double>> B(M, vector<double>(L));
    vector<vector<double>> C_sequential(N, vector<double>(L));
    vector<vector<double>> C_parallel(N, vector<double>(L));

    // Fill matrices with random values
    cout << "Initializing matrices..." << endl;
    initializeMatrix(A, N, M);
    initializeMatrix(B, M, L);

    // Run sequential multiplication and measure time
    cout << "Running sequential matrix multiplication..." << endl;
    auto start_seq = high_resolution_clock::now();
    sequentialMatrixMultiplication(A, B, C_sequential);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<milliseconds>(end_seq - start_seq);
    cout << "Sequential execution time: " << duration_seq.count() << " ms" << endl;

    // Run parallel multiplication with different thread counts and measure time
    vector<int> thread_counts = {2, 4, 8, 16};
    
    for (int num_threads : thread_counts) {
        cout << "Running parallel matrix multiplication with " << num_threads << " threads..." << endl;
        auto start_par = high_resolution_clock::now();
        parallelMatrixMultiplication(A, B, C_parallel, num_threads);
        auto end_par = high_resolution_clock::now();
        auto duration_par = duration_cast<milliseconds>(end_par - start_par);
        cout << "Parallel execution time with " << num_threads << " threads: " 
             << duration_par.count() << " ms" << endl;
        
        // Verify correctness
        if (verifyResults(C_sequential, C_parallel)) {
            cout << "Results verified: Sequential and parallel outputs match." << endl;
            
            // Calculate speedup
            double speedup = static_cast<double>(duration_seq.count()) / duration_par.count();
            cout << "Speedup with " << num_threads << " threads: " << speedup << "x" << endl;
        } else {
            cout << "ERROR: Results do not match!" << endl;
        }
        cout << "-------------------------------------------" << endl;
    }

    return 0;
}