#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <queue>
#include <vector>
#include <string>
#include "Order.h"

struct CompareSellOrders {
    bool operator()(const Order& o1, const Order& o2);
};

struct CompareBuyOrders {
    bool operator()(const Order& o1, const Order& o2);
};

class OrderBook {
private:
    std::priority_queue<Order, std::vector<Order>, CompareBuyOrders> buyOrders;
    std::priority_queue<Order, std::vector<Order>, CompareSellOrders> sellOrders;

    void cancel(std::string id);
    void executeBuyLimitOrder(Order& newOrder);
    void executeSellLimitOrder(Order& newOrder);
    void executeBuyMarketOrder(Order& newOrder);
    void executeSellMarketOrder(Order& newOrder);

public:
    void parseInput();
    void print();
};

#endif  // ORDERBOOK_H
