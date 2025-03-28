#include "coffeeshop.h"
#include <vector>
#include <thread>
#include <chrono>

const int CUSTOMERS = 10;
const int BARISTAS = 2;
const size_t QUEUE_LENGTH = 7;

void simulateCustomers(const std::vector<int>& orderLengths) {
    coffeeQueue shopQueue(QUEUE_LENGTH);

    std::vector<std::thread> baristaThreads;
    //run the barista function using as many threads as defined above
    for (int i = 0; i < BARISTAS; ++i) {
        baristaThreads.emplace_back(barista, std::ref(shopQueue));
    }
    
    std::vector<std::thread> customerThreads;
    for (int i = 0; i < CUSTOMERS; ++i) {
        coffeeOrder customerOrder;
        customerOrder.prepTime = orderLengths[i];
        customerOrder.id = i + 1;
        customerThreads.emplace_back(customer, std::ref(shopQueue), customerOrder);
    }

    for (auto &t : customerThreads) {
        if (t.joinable()) {
            t.join();
        }
    }

    // once all customers done, shut down baristas
    for (int i = 0; i < BARISTAS; ++i) {
        coffeeOrder killOrder;
        killOrder.prepTime = 0;
        killOrder.id = -1;
        
        while (!shopQueue.enqueue(killOrder)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    for (auto &t : baristaThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

int main() {
    std::vector<int> orderLengths(CUSTOMERS);
    for (int i = 1; i <= CUSTOMERS; i++) {
        std::cout << "Enter order length (in seconds) for customer " << (i) << ": ";
        std::cin >> orderLengths[i];
    }

    simulateCustomers(orderLengths);
}
