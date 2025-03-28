#ifndef COFFEESHOP_H
#define COFFEESHOP_H

#include <mutex>
#include <iostream>
#include <queue>
#include <chrono>

//Struct for an order. 
struct coffeeOrder {
    int prepTime; // time in seconds order will take to prep
    int id = 0;
};

class coffeeQueue {
    public:
        coffeeQueue(size_t maxCapacity);
        bool enqueue(const coffeeOrder &order);
        coffeeOrder dequeue();
        bool isEmpty();
    private:
        size_t maxOrders;
        std::mutex coffeeMutex;
        std::queue<coffeeOrder> orders;
};

void customer(coffeeQueue &queue, coffeeOrder order);
void barista(coffeeQueue &queue);

#endif
