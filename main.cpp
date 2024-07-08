#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

// Assuming these headers are correctly defined and accessible
#include "LockFreeSkipList.h"
#include "MarketDataSimulator.h"

void performanceTest() {
    // Initialize the lock-free skip list with max level and probability
    LockFreeSkipList<int> orderBook(16, 0.5);

    // Initialize the market data simulator
    MarketDataSimulator simulator;

    // Record the start time
    auto start = std::chrono::high_resolution_clock::now();

    // Create a vector to hold the threads
    std::vector<std::thread> threads;

    // Launch 10 threads to generate orders concurrently
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&orderBook, &simulator]() {
            simulator.generateOrders(orderBook, 1000);
        });
    }

    // Join all the threads to ensure they complete execution
    for (auto& t : threads) {
        t.join();
    }

    // Record the end time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> diff = end - start;

    // Output the time taken
    std::cout << "Time taken: " << diff.count() << " seconds" << std::endl;
}

int main() {
    // Run the performance test
    performanceTest();
    return 0;
}
