#ifndef MARKETDATASIMULATOR_H
#define MARKETDATASIMULATOR_H

#include "LockFreeSkipList.h"

class MarketDataSimulator {
public:
    void generateOrders(LockFreeSkipList<int>& orderBook, int numOrders);
    void cancelOrders(LockFreeSkipList<int>& orderBook, int numOrders);
};

#endif // MARKETDATASIMULATOR_H
