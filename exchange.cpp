#include <iostream>
#include <string>
#include "Order.h"
#include "OrderBook.h"

int main() {
    OrderBook orderBook;

    orderBook.parseInput();
    orderBook.print();

    return 0;
}
