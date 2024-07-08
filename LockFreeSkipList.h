#ifndef LOCKFREESKIPLIST_H
#define LOCKFREESKIPLIST_H

#include <atomic>
#include <vector>
#include <limits>
#include <random>
#include <iostream>

template<typename T>
class LockFreeSkipList {
    struct Node {
        T value;
        std::vector<std::atomic<Node*>> forward;

        Node(int level, T val) : value(val), forward(level + 1) {}
    };

    std::atomic<Node*> head;
    int maxLevel;
    float probability;
    std::atomic<int> level;

public:
    LockFreeSkipList(int maxLvl, float prob);
    int randomLevel();
    bool insert(T value);
    bool remove(T value);
    void printList();
};

#include "LockFreeSkipList.tpp" // Include template implementation

#endif // LOCKFREESKIPLIST_H
