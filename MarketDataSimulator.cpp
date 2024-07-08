#include "MarketDataSimulator.h"
#include <random>

void MarketDataSimulator::generateOrders(LockFreeSkipList<int>& orderBook, int numOrders) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

    for (int i = 0; i < numOrders; ++i) {
        int order = dist(gen);
        orderBook.insert(order);
    }
}

void MarketDataSimulator::cancelOrders(LockFreeSkipList<int>& orderBook, int numOrders) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

    for (int i = 0; i < numOrders; ++i) {
        int order = dist(gen);
        orderBook.remove(order);
    }
}
