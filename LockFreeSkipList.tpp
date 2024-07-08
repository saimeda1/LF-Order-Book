#include "LockFreeSkipList.h"

template<typename T>
LockFreeSkipList<T>::LockFreeSkipList(int maxLvl, float prob) : maxLevel(maxLvl), probability(prob), level(0) {
    head = new Node(maxLvl, std::numeric_limits<T>::min());
    for (int i = 0; i <= maxLvl; ++i) {
        head.load()->forward[i] = nullptr;
    }
}

template<typename T>
int LockFreeSkipList<T>::randomLevel() {
    int lvl = 0;
    while (static_cast<float>(rand()) / RAND_MAX < probability && lvl < maxLevel) {
        ++lvl;
    }
    return lvl;
}

template<typename T>
bool LockFreeSkipList<T>::insert(T value) {
    std::vector<Node*> update(maxLevel + 1);
    Node* x = head.load();

    for (int i = level.load(); i >= 0; --i) {
        while (x->forward[i].load() && x->forward[i].load()->value < value) {
            x = x->forward[i].load();
        }
        update[i] = x;
    }

    int lvl = randomLevel();
    if (lvl > level.load()) {
        for (int i = level.load() + 1; i <= lvl; ++i) {
            update[i] = head.load();
        }
        level.store(lvl);
    }

    Node* newNode = new Node(lvl, value);
    for (int i = 0; i <= lvl; ++i) {
        newNode->forward[i].store(update[i]->forward[i].load());
        update[i]->forward[i].store(newNode);
    }
    return true;
}

template<typename T>
bool LockFreeSkipList<T>::remove(T value) {
    std::vector<Node*> update(maxLevel + 1);
    Node* x = head.load();

    for (int i = level.load(); i >= 0; --i) {
        while (x->forward[i].load() && x->forward[i].load()->value < value) {
            x = x->forward[i].load();
        }
        update[i] = x;
    }

    x = x->forward[0].load();
    if (!x || x->value != value) {
        return false;
    }

    for (int i = 0; i <= level.load(); ++i) {
        if (update[i]->forward[i].load() != x) {
            break;
        }
        update[i]->forward[i].store(x->forward[i].load());
    }

    while (level.load() > 0 && !head.load()->forward[level.load()].load()) {
        level.store(level.load() - 1);
    }

    delete x;
    return true;
}

template<typename T>
void LockFreeSkipList<T>::printList() {
    for (int i = 0; i <= level.load(); ++i) {
        Node* node = head.load()->forward[i].load();
        std::cout << "Level " << i << ": ";
        while (node != nullptr) {
            std::cout << node->value << " ";
            node = node->forward[i].load();
        }
        std::cout << std::endl;
    }
}
