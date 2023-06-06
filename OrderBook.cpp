#include <iostream>
#include <sstream>
#include "OrderBook.h"

bool CompareBuyOrders::operator()(const Order& o1, const Order& o2) {
    if (o1.price > o2.price)
        return false;
    else if (o1.price == o2.price && o1.orderOfInsertion < o2.orderOfInsertion)
        return false;
    return true;
}

bool CompareSellOrders::operator()(const Order& o1, const Order& o2) {
    if (o1.price < o2.price)
        return true;
    else if (o1.price == o2.price && o1.orderOfInsertion < o2.orderOfInsertion)
        return true;
    return false;
}

void OrderBook::cancel(std::string id) {
    std::priority_queue<Order, std::vector<Order>, CompareSellOrders> tempSellOrders = sellOrders;
    std::priority_queue<Order, std::vector<Order>, CompareBuyOrders> tempBuyOrders = buyOrders;

    std::vector<Order> remainingS;
    while (!tempSellOrders.empty()) {
        Order curr = tempSellOrders.top();
        tempSellOrders.pop();
        if (curr.orderId == id) {
            continue;
        }
        remainingS.push_back(curr);
    }
    sellOrders = std::priority_queue<Order, std::vector<Order>, CompareSellOrders>(remainingS.begin(), remainingS.end());

    std::vector<Order> remainingB;
    while (!tempBuyOrders.empty()) {
        Order curr = tempBuyOrders.top();
        tempBuyOrders.pop();
        if (curr.orderId == id) {
            continue;
        }
        remainingB.push_back(curr);
    }
    buyOrders = std::priority_queue<Order, std::vector<Order>, CompareBuyOrders>(remainingB.begin(), remainingB.end());
}

void OrderBook::executeSellLimitOrder(Order& newOrder) {
    int transaction = 0;
    std::vector<Order> remainingOrders;

    std::priority_queue<Order, std::vector<Order>, CompareBuyOrders> tempBuyOrders = buyOrders;
    while (!tempBuyOrders.empty()) {
        Order curr = tempBuyOrders.top();
        tempBuyOrders.pop();

        if (curr.price >= newOrder.price) {
            int minQnt = std::min(newOrder.quantity, curr.quantity);
            transaction += minQnt * curr.price;
            newOrder.quantity -= minQnt;
            curr.quantity -= minQnt;
        }

        if (curr.quantity != 0) {
            remainingOrders.push_back(curr);
        }

        if (newOrder.quantity == 0) {
            break;
        }
    }

    buyOrders = std::priority_queue<Order, std::vector<Order>, CompareBuyOrders>(remainingOrders.begin(), remainingOrders.end());

    if (newOrder.quantity != 0) {
        sellOrders.push(newOrder);
    }

    std::cout << transaction << std::endl;
}

void OrderBook::executeBuyLimitOrder(Order& newOrder) {
    int transaction = 0;
    std::vector<Order> remainingOrders;

    std::priority_queue<Order, std::vector<Order>, CompareSellOrders> tempSellOrders = sellOrders;
    while (!tempSellOrders.empty()) {
        Order curr = tempSellOrders.top();
        tempSellOrders.pop();
        
        if (newOrder.price >= curr.price) {
            int minQnt = std::min(newOrder.quantity, curr.quantity);
            transaction += minQnt * curr.price;
            newOrder.quantity -= minQnt;
            curr.quantity -= minQnt;
        }

        if (curr.quantity != 0) {
            remainingOrders.push_back(curr);
        }

        if (newOrder.quantity == 0) {
            break;
        }
    }

    sellOrders = std::priority_queue<Order, std::vector<Order>, CompareSellOrders>(remainingOrders.begin(), remainingOrders.end());
    

    if (newOrder.quantity != 0) {
        buyOrders.push(newOrder);
    }

    std::cout << transaction << std::endl;
}

void OrderBook::executeBuyMarketOrder(Order& newOrder) {
    int transaction = 0;
    std::vector<Order> removed;

    std::priority_queue<Order, std::vector<Order>, CompareSellOrders> tempSellOrder = sellOrders;
    while (!tempSellOrder.empty()) {
        Order curr = tempSellOrder.top();
        tempSellOrder.pop();

        int minQnt = std::min(newOrder.quantity, curr.quantity);
        transaction += minQnt * curr.price;
        newOrder.quantity -= minQnt;
        curr.quantity -= minQnt;

        if (curr.quantity != 0) {
            removed.push_back(curr);
        }

        if (newOrder.quantity == 0) {
            break;
        }
    }
    sellOrders = std::priority_queue<Order, std::vector<Order>, CompareSellOrders>(removed.begin(), removed.end());

    std::cout << transaction << std::endl;
}

void OrderBook::executeSellMarketOrder(Order& newOrder) {
    int transaction = 0;
    std::vector<Order> removed;

    std::priority_queue<Order, std::vector<Order>, CompareBuyOrders> tempBuyOrders = buyOrders;
    while (!tempBuyOrders.empty()) {
        Order curr = tempBuyOrders.top();
        tempBuyOrders.pop();

        int minQnt = std::min(newOrder.quantity, curr.quantity);
        transaction += minQnt * curr.price;
        newOrder.quantity -= minQnt;
        curr.quantity -= minQnt;

        if (curr.quantity != 0) {
            removed.push_back(curr);
        }

        if (newOrder.quantity == 0) {
            break;
        }
    }
    buyOrders = std::priority_queue<Order, std::vector<Order>, CompareBuyOrders>(removed.begin(), removed.end());

    std::cout << transaction << std::endl;
}

void OrderBook::parseInput() {
    int countOfInsert = 0;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "END") {
            break;
        }

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> splitLine;
        while (std::getline(iss, token, ' ')) {
            splitLine.push_back(token);
        }

        if (splitLine.size() == 6) {
            if (splitLine[2] == "B") {
                Order newOrder(std::stoi(splitLine[5]), countOfInsert, splitLine[3], std::stoi(splitLine[4]), true);
                executeBuyLimitOrder(newOrder);
            } else {
                Order newOrder(std::stoi(splitLine[5]), countOfInsert, splitLine[3], std::stoi(splitLine[4]), false);
                executeSellLimitOrder(newOrder);
            }
            countOfInsert++;
        } else if (splitLine.size() == 5) {
            if (splitLine[2] == "B") {
                Order newOrder(-1, countOfInsert, splitLine[3], std::stoi(splitLine[4]), true);
                executeBuyMarketOrder(newOrder);
            } else {
                Order newOrder(-1, countOfInsert, splitLine[3], std::stoi(splitLine[4]), false);
                executeSellMarketOrder(newOrder);
            }
            countOfInsert++;
        } else {
            std::string orderId = splitLine[1];
            cancel(orderId);
        }
    }
}

void OrderBook::print() {
    std::priority_queue<Order, std::vector<Order>, CompareBuyOrders> tempBuyOrders = buyOrders;
    std::priority_queue<Order, std::vector<Order>, CompareSellOrders> tempSellOrders = sellOrders;

    std::cout << "B: ";
    while (!tempBuyOrders.empty()) {
        Order b = tempBuyOrders.top();
        tempBuyOrders.pop();
        std::cout << b << " ";
    }
    std::cout << std::endl;

    std::cout << "S: ";
    while (!tempSellOrders.empty()) {
        Order s = tempSellOrders.top();
        tempSellOrders.pop();
        std::cout << s << " ";
    }
    std::cout << std::endl;
}


