#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <mutex>
#include <queue>
#include <iomanip>

// The Riemann Zeta function as given in the problem
double Riemann_Zeta(double s, uint64_t k) {
    double result = 0.0;
    
    for (uint64_t i = 1; i < k; i++)
        for (uint64_t j = 1; j < k; j++)
            result += (2*(i&1)-1)/std::pow(i+j, s);
    
    return result*std::pow(2, s);
}

// Static block-cyclic distribution
void static_block_cyclic(std::vector<double>& X, int chunk_size, int num_threads) {
    std::vector<std::thread> threads(num_threads);
    
    for (int t = 0; t < num_threads; t++) {
        threads[t] = std::thread([t, &X, chunk_size, num_threads]() {
            uint64_t total_chunks = (X.size() + chunk_size - 1) / chunk_size;
            for (uint64_t chunk = t; chunk < total_chunks; chunk += num_threads) {
                uint64_t start = chunk * chunk_size;
                uint64_t end = std::min(start + chunk_size, X.size());
                for (uint64_t k = start; k < end; k++) {
                    X[k] = Riemann_Zeta(2, k);
                }
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
}

// Dynamic block-cyclic distribution
void dynamic_block_cyclic(std::vector<double>& X, int chunk_size, int num_threads) {
    std::vector<std::thread> threads(num_threads);
    
    std::mutex queue_mutex;
    std::queue<uint64_t> chunk_queue;
    
    // Initialize the queue with chunks
    for (uint64_t start = 0; start < X.size(); start += chunk_size) {
        chunk_queue.push(start);
    }
    
    auto worker_function = [&X, chunk_size, &chunk_queue, &queue_mutex]() {
        while (true) {
            uint64_t start;
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                if (chunk_queue.empty()) {
                    break;
                }
                start = chunk_queue.front();
                chunk_queue.pop();
            }
            
            uint64_t end = std::min(start + chunk_size, X.size());
            for (uint64_t k = start; k < end; k++) {
                X[k] = Riemann_Zeta(2, k);
            }
        }
    };
    
    for (int t = 0; t < num_threads; t++) {
        threads[t] = std::thread(worker_function);
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
}

int main() {
    const uint64_t n = 2048;
    const int num_threads = 8;
    std::vector<double> X(n);
    
    std::cout << "Riemann Zeta Function Parallelization Comparison\n";
    std::cout << "n = " << n << ", Number of threads = " << num_threads << "\n\n";
    
    // Sequential computation for reference
    auto start = std::chrono::high_resolution_clock::now();
    for (uint64_t k = 0; k < n; k++)
        X[k] = Riemann_Zeta(2, k);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Sequential time: " << diff.count() << " s\n\n";
    
    // Test static block-cyclic with different chunk sizes
    std::cout << "Static Block-Cyclic Distribution:\n";
    std::cout << "--------------------------------\n";
    for (int chunk_size : {1, 2, 4, 8}) {
        std::vector<double> X_static(n);
        start = std::chrono::high_resolution_clock::now();
        static_block_cyclic(X_static, chunk_size, num_threads);
        end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "Chunk size " << chunk_size << ": " << diff.count() << " s\n";
    }
    
    std::cout << "\nDynamic Block-Cyclic Distribution:\n";
    std::cout << "---------------------------------\n";
    // Test dynamic block-cyclic with different chunk sizes
    for (int chunk_size : {1, 2, 4, 8}) {
        std::vector<double> X_dynamic(n);
        start = std::chrono::high_resolution_clock::now();
        dynamic_block_cyclic(X_dynamic, chunk_size, num_threads);
        end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        std::cout << "Chunk size " << chunk_size << ": " << diff.count() << " s\n";
    }
    
    // Print a subset of the results for verification
    std::cout << "\nSubset of results for verification:\n";
    std::cout << "---------------------------------\n";
    for (uint64_t k : {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2047}) {
        std::cout << "X[" << std::setw(4) << k << "] = " << X[k] << "\n";
    }
    
    return 0;
}


//Share Riemann Zeta Parallelization