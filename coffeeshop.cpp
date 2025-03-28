#include "coffeeshop.h"
#include <chrono>
#include <thread>

//change this value to max wait time for customers (in seconds)
const int MAX_WAIT_TIME = 10;
//change this value to max length order a barista will take (in seconds)
const int MAX_ORDER_TIME = 60;

//doesn't really need much, we just want to set capacity
coffeeQueue::coffeeQueue(size_t maxCapacity) : maxOrders(maxCapacity) {

}

bool coffeeQueue::isEmpty() {
    return (orders.size() == 0);
}

bool coffeeQueue::enqueue(const coffeeOrder &order) {
    coffeeMutex.lock();
    //if queue is full, return false
    if (maxOrders <= orders.size()) {
        return false; 
    }
    orders.push(order);
    coffeeMutex.unlock();
    return true;
}

coffeeOrder coffeeQueue::dequeue() {
    coffeeMutex.lock();
    coffeeOrder order;
    if (orders.empty()) {
        std::cerr << "Barista tried to dequeue an empty queue!" << std::endl;
        coffeeMutex.unlock();
        // if no order in queue, return order w prep time of 0. Barista will skip
        order.prepTime = 0;
        order.id = -2;
        return order;
    }
    //grab element from front of queue
    order = orders.front();
    orders.pop();
    coffeeMutex.unlock();
    return order;
}

void barista(coffeeQueue &coffeeQueue) {
    std::cout << "Barista has clocked in!\n";
    while (1) {
        coffeeOrder baristaOrder = coffeeQueue.dequeue();
        if (baristaOrder.prepTime > 0) {
            //if order takes too long, decline
            if (baristaOrder.prepTime > MAX_ORDER_TIME) {
                std::cout << "Barista has declined order " << baristaOrder.id << "\n";
            }
            else {
                // wait for prep time, 
                std::this_thread::sleep_for(std::chrono::seconds(baristaOrder.prepTime));
                std::cout << "Barista has completed order " << baristaOrder.id << "\n!";
            }
        } else {
            if (baristaOrder.id == -1) { //if it's time to shut down barista, give it order id -1
                std::cout << "A barista has clocked out!\n";
                break;
            };
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void customer(coffeeQueue &coffeeQueue, coffeeOrder order) {
    bool orderPlaced = false;
    auto begin = std::chrono::steady_clock::now();
    //we continually try to place an order. Start tracking time from first attempt.
    while (!orderPlaced) {
        if (std::chrono::steady_clock::now() - begin > std::chrono::seconds(MAX_WAIT_TIME)) {
            std::cout << "Customer with order number " << order.id << " waited too long and left. \n";
        }
        orderPlaced = coffeeQueue.enqueue(order);
    }
    std::cout << "Customer has placed order number " << order.id << "\n";
}