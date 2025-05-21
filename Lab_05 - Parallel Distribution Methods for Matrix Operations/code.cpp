#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <cmath>

const int N = 2048;
const int NUM_THREADS = 8;
const int BLOCK_SIZE = 64;

// Structs for thread data
struct BlockData {
    float* A;
    float* B;
    float* C;
    int start_row;
    int end_row;
    int N;
};

struct CyclicData {
    float* A;
    float* B;
    float* C;
    int thread_id;
    int num_threads;
    int N;
};

struct BlockCyclicData {
    float* A;
    float* B;
    float* C;
    int thread_id;
    int num_threads;
    int block_size;
    int N;
};

// Thread functions
void subtract_block(BlockData& data) {
    for (int i = data.start_row; i < data.end_row; ++i) {
        for (int j = 0; j < data.N; ++j) {
            data.C[i * data.N + j] = data.A[i * data.N + j] - data.B[i * data.N + j];
        }
    }
}

void subtract_cyclic(CyclicData& data) {
    for (int i = data.thread_id; i < data.N; i += data.num_threads) {
        for (int j = 0; j < data.N; ++j) {
            data.C[i * data.N + j] = data.A[i * data.N + j] - data.B[i * data.N + j];
        }
    }
}

void subtract_block_cyclic(BlockCyclicData& data) {
    int total_blocks = (data.N + data.block_size - 1) / data.block_size;
    for (int block = data.thread_id; block < total_blocks; block += data.num_threads) {
        int start_row = block * data.block_size;
        int end_row = std::min(start_row + data.block_size, data.N);
        for (int i = start_row; i < end_row; ++i) {
            for (int j = 0; j < data.N; ++j) {
                data.C[i * data.N + j] = data.A[i * data.N + j] - data.B[i * data.N + j];
            }
        }
    }
}

bool verify(float* C1, float* C2, int size) {
    for (int i = 0; i < size; ++i) {
        if (std::fabs(C1[i] - C2[i]) > 1e-5) {
            std::cerr << "Verification failed at index " << i << ": " << C1[i] << " vs " << C2[i] << std::endl;
            return false;
        }
    }
    return true;
}

int main() {
    float* A = new float[N * N];
    float* B = new float[N * N];
    float* C_serial = new float[N * N]();
    float* C_block = new float[N * N]();
    float* C_cyclic = new float[N * N]();
    float* C_block_cyclic = new float[N * N]();

    // Initialize A and B with random values
    std::default_random_engine gen(42);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (int i = 0; i < N * N; ++i) {
        A[i] = dist(gen);
        B[i] = dist(gen);
    }

    // Serial subtraction
    auto start_serial = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N * N; ++i) {
        C_serial[i] = A[i] - B[i];
    }
    auto end_serial = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_serial = end_serial - start_serial;

    // Block distribution
    std::vector<std::thread> threads;
    BlockData block_data[NUM_THREADS];
    int rows_per_thread = N / NUM_THREADS;
    int remainder = N % NUM_THREADS;

    auto start_block = std::chrono::high_resolution_clock::now();
    int current_start = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        int extra = (i < remainder) ? 1 : 0;
        block_data[i] = {A, B, C_block, current_start, current_start + rows_per_thread + extra, N};
        current_start = block_data[i].end_row;
        threads.emplace_back(subtract_block, std::ref(block_data[i]));
    }

    for (auto& t : threads) {
        t.join();
    }
    auto end_block = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_block = end_block - start_block;

    if (!verify(C_serial, C_block, N * N)) {
        std::cerr << "Block result mismatch!" << std::endl;
    }

    // Cyclic distribution
    threads.clear();
    CyclicData cyclic_data[NUM_THREADS];
    auto start_cyclic = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; ++i) {
        cyclic_data[i] = {A, B, C_cyclic, i, NUM_THREADS, N};
        threads.emplace_back(subtract_cyclic, std::ref(cyclic_data[i]));
    }

    for (auto& t : threads) {
        t.join();
    }
    auto end_cyclic = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_cyclic = end_cyclic - start_cyclic;

    if (!verify(C_serial, C_cyclic, N * N)) {
        std::cerr << "Cyclic result mismatch!" << std::endl;
    }

    // Block-cyclic distribution
    threads.clear();
    BlockCyclicData block_cyclic_data[NUM_THREADS];
    auto start_block_cyclic = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; ++i) {
        block_cyclic_data[i] = {A, B, C_block_cyclic, i, NUM_THREADS, BLOCK_SIZE, N};
        threads.emplace_back(subtract_block_cyclic, std::ref(block_cyclic_data[i]));
    }

    for (auto& t : threads) {
        t.join();
    }
    auto end_block_cyclic = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_block_cyclic = end_block_cyclic - start_block_cyclic;

    if (!verify(C_serial, C_block_cyclic, N * N)) {
        std::cerr << "Block-cyclic result mismatch!" << std::endl;
    }

    // Output results
    std::cout << "Serial Time: " << elapsed_serial.count() << " s\n";
    std::cout << "Block Distribution Time: " << elapsed_block.count() << " s\n";
    std::cout << "Cyclic Distribution Time: " << elapsed_cyclic.count() << " s\n";
    std::cout << "Block-Cyclic Distribution Time: " << elapsed_block_cyclic.count() << " s\n";

    delete[] A;
    delete[] B;
    delete[] C_serial;
    delete[] C_block;
    delete[] C_cyclic;
    delete[] C_block_cyclic;

    return 0;
}