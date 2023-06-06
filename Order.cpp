#include "Order.h"
#include <iostream>

Order::Order(int price, int orderOfInsertion, std::string orderId, int quantity, bool isBuy)
    : price(price), orderOfInsertion(orderOfInsertion), orderId(orderId), quantity(quantity), isBuy(isBuy) {}

std::ostream& operator<<(std::ostream& os, const Order& obj) {
    os << obj.quantity << "@" << obj.price << "#" << obj.orderId;
    return os;
}

