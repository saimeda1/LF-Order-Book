#ifndef ORDERMATCHINGENGINE_H
#define ORDERMATCHINGENGINE_H

#include "LockFreeSkipList.h"

class OrderMatchingEngine {
    LockFreeSkipList<int>& buyOrders;
    LockFreeSkipList<int>& sellOrders;

public:
    OrderMatchingEngine(LockFreeSkipList<int>& buys, LockFreeSkipList<int>& sells);
    void matchOrders();
};

#endif // ORDERMATCHINGENGINE_H
