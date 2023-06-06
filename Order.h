#ifndef ORDER_H
#define ORDER_H

#include <string>

class Order {
public:
    int price;
    int orderOfInsertion;
    std::string orderId;
    int quantity;
    bool isBuy;

    Order(int price, int orderOfInsertion, std::string orderId, int quantity, bool isBuy);
    
    // Overloaded << operator (declaration)
    friend std::ostream& operator<<(std::ostream& os, const Order& obj);
};

#endif  // ORDER_H
