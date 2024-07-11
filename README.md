# LF-Order-Book


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
Concurrency Handling

Uses fine-grained locking to reduce contention and improve performance. Mutexes are used to synchronize access to the bids and asks maps.

Market Data Simulation

Simulates market conditions by generating random orders and feeding them into the order book.

cpp
Copy code
void simulate_order_input(OrderBook& book, int num_orders, char order_type) {
    for (int i = 0; i < num_orders; ++i) {
        double price = 100 + rand() % 10 - 5;
        Order new_order(i, price, 1 + rand() % 10, order_type, std::chrono::system_clock::now().time_since_epoch().count());
        book.add_order(new_order);
    }
}
Order Matching Engine

Matches orders based on price-time priority, ensuring fair and efficient execution.

cpp
Copy code
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
Further Enhancements and Considerations
Fine-Grained Locking: Reduces contention between threads adding orders and the matching engine.
Concurrency Management: Ensures deadlock avoidance with std::lock and multiple mutexes.
Market Simulation: Involves random pricing and quantities for realistic simulation.
Lock-Free Transition: Replace maps and deques with lock-free data structures and use atomic operations for managing pointers in these structures.
