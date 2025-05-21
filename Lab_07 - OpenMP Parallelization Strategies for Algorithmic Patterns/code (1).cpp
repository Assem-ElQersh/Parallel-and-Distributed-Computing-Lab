#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Problem size
const int N = 1024;  // Number of items
const int C = 1024;  // Knapsack capacity

// Structure to represent an item
struct Item {
    int weight;
    int value;
};

// Function to initialize items with random values
void initializeItems(vector<Item>& items) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> weight_dist(1, C/10);  // Weights between 1 and C/10
    uniform_int_distribution<> value_dist(1, 100);    // Values between 1 and 100

    for (int i = 0; i < N; i++) {
        items[i].weight = weight_dist(gen);
        items[i].value = value_dist(gen);
    }
}

// Sequential knapsack using dynamic programming
int sequentialKnapsack(const vector<Item>& items, int capacity) {
    // Initialize DP table
    vector<vector<int>> dp(N + 1, vector<int>(capacity + 1, 0));
    
    // Fill the dp table
    for (int i = 1; i <= N; i++) {
        for (int w = 0; w <= capacity; w++) {
            // Don't include item i
            dp[i][w] = dp[i-1][w];
            
            // Include item i if it fits
            if (items[i-1].weight <= w) {
                dp[i][w] = max(dp[i][w], 
                               dp[i-1][w - items[i-1].weight] + items[i-1].value);
            }
        }
    }
    
    return dp[N][capacity];
}

// Parallel knapsack using dynamic programming with OpenMP
int parallelKnapsack(const vector<Item>& items, int capacity, int num_threads) {
    // Initialize DP table
    vector<vector<int>> dp(N + 1, vector<int>(capacity + 1, 0));
    
    // Set number of threads
    omp_set_num_threads(num_threads);
    
    // Fill the dp table
    for (int i = 1; i <= N; i++) {
        // Parallelize the inner loop
        #pragma omp parallel for
        for (int w = 0; w <= capacity; w++) {
            // Don't include item i
            dp[i][w] = dp[i-1][w];
            
            // Include item i if it fits
            if (items[i-1].weight <= w) {
                dp[i][w] = max(dp[i][w], 
                               dp[i-1][w - items[i-1].weight] + items[i-1].value);
            }
        }
    }
    
    return dp[N][capacity];
}

int main() {
    // Initialize items
    vector<Item> items(N);
    cout << "Initializing " << N << " items for knapsack problem..." << endl;
    initializeItems(items);
    
    // Run sequential knapsack and measure time
    cout << "Running sequential knapsack..." << endl;
    auto start_seq = high_resolution_clock::now();
    int max_value_seq = sequentialKnapsack(items, C);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<milliseconds>(end_seq - start_seq);
    cout << "Sequential execution time: " << duration_seq.count() << " ms" << endl;
    cout << "Maximum value (sequential): " << max_value_seq << endl;
    
    // Run parallel knapsack with different thread counts and measure time
    vector<int> thread_counts = {2, 4, 8, 16};
    
    for (int num_threads : thread_counts) {
        cout << "Running parallel knapsack with " << num_threads << " threads..." << endl;
        auto start_par = high_resolution_clock::now();
        int max_value_par = parallelKnapsack(items, C, num_threads);
        auto end_par = high_resolution_clock::now();
        auto duration_par = duration_cast<milliseconds>(end_par - start_par);
        cout << "Parallel execution time with " << num_threads << " threads: " 
             << duration_par.count() << " ms" << endl;
        cout << "Maximum value (parallel): " << max_value_par << endl;
        
        // Verify correctness
        if (max_value_seq == max_value_par) {
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