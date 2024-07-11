# LF Order Book

LF Order Book is a lock-free order book implementation designed for high-performance trading systems. This project provides a robust and efficient way to manage buy and sell orders, ensuring accurate and timely execution of trades without traditional locking mechanisms.

## Features

- **Efficient Data Structure**: Utilizes advanced data structures to maintain and query order books efficiently.
- **Lock-Free Algorithms**: Implements lock-free algorithms to manage shared data.
- **High Throughput**: Designed to handle a large number of orders with minimal latency.
- **Concurrency Handling**: Ensures thread safety through atomic operations and lock-free programming techniques.

## Getting Started

### Prerequisites

- C++11 or later
- CMake (for building the project)
- Standard C++ Library

### Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/saimeda1/LF-Order-Book.git
    cd LF-Order-Book
    ```

2. Build the project using CMake:

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

### Usage

1. Run the main executable to start the order book:

    ```bash
    ./Meda_LF_Book
    ```

2. The application will simulate order inputs and start the matching engine.

# Code Overview

## Order Structure

Defines an order with attributes such as order_id, price, quantity, order_type (buy/sell), and a timestamp for price-time priority.

```c++
struct Order {
    uint64_t order_id;
    double price;
    int quantity;
    char order_type; // 'B' for buy, 'S' for sell
    uint64_t timestamp;

    Order(uint64_t id, double p, int q, char type, uint64_t time)
        : order_id(id), price(p), quantity(q), order_type(type), timestamp(time) {}
};
```


## Order Book

Manages buy and sell orders using separate maps for bids and asks.

```c++
class OrderBook {
public:
    std::map<double, std::deque<Order>> bids;
    std::map<double, std::deque<Order>> asks;
    std::mutex mtx_bids;
    std::mutex mtx_asks;

    void add_order(Order order);
    void match_orders();
};
```

## Concurrency Handling

Uses fine-grained locking to reduce contention and improve performance. Mutexes are used to synchronize access to the bids and asks maps.

Market Data Simulation

Simulates market conditions by generating random orders and feeding them into the order book.

```c++
void simulate_order_input(OrderBook& book, int num_orders, char order_type) {
    for (int i = 0; i < num_orders; ++i) {
        double price = 100 + rand() % 10 - 5;
        Order new_order(i, price, 1 + rand() % 10, order_type, std::chrono::system_clock::now().time_since_epoch().count());
        book.add_order(new_order);
    }
}
```
## Order Matching Engine

Matches orders based on price-time priority, ensuring fair and efficient execution.


```c++
void match_orders(OrderBook& book) {
    std::lock(book.mtx_bids, book.mtx_asks);
    std::lock_guard<std::mutex> lock_bids(book.mtx_bids, std::adopt_lock);
    std::lock_guard<std::mutex> lock_asks(book.mtx_asks, std::adopt_lock);

    while (!book.bids.empty() && !book.asks.empty()) {
        auto& best_bid = book.bids.rbegin()->second.front();
        auto& best_ask = book.asks.begin()->second.front();

        if (best_bid.price >= best_ask.price) {
            int matched_quantity = std::min(best_bid.quantity, best_ask.quantity);
            std::cout << "Matched Order: Buy " << best_bid.order_id << " with Sell " << best_ask.order_id << " for " << matched_quantity << " units." << std::endl;
            
            best_bid.quantity -= matched_quantity;
            best_ask.quantity -= matched_quantity;

            if (best_bid.quantity == 0) {
                book.bids.rbegin()->second.pop_front();
                if (book.bids.rbegin()->second.empty()) {
                    book.bids.erase(--book.bids.end());
                }
            }
            if (best_ask.quantity == 0) {
                book.asks.begin()->second.pop_front();
                if (book.asks.begin()->second.empty()) {
                    book.asks.erase(book.asks.begin());
                }
            }
        } else {
            break;
        }
    }
}
```


