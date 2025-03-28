#ifndef COFFEESHOP_H
#define COFFEESHOP_H

#include <mutex>
#include <iostream>

//Struct for an order. 
struct coffeeOrder {
    int prepTime;
};

class coffeeQueue {
    public:
        coffeeQueue(size_t maxCapacity);
        bool enqueue(const coffeeOrder &order);
        bool dequeue(coffeeOrder &order);
    private:
        size_t maxOrders;
        std::mutex coffeeMutex;
};

void customer(coffeeQueue &queue, coffeeOrder order);
void barista(coffeeQueue &queue);

#endif
