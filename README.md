# SimpleExchange

##### What is this?
This is a simple simulation of an order matching engine that matches "buy" and "sell" orders for a stock market.
A trader can submit several types of orders to a matching engine, which will process and possibly place them into an Order Book.
The matching engine will support different types of submitted orders, as well as a few other possible actions that a Trader might want to perform.

###### Details
The OB is a list of buy and sell orders. 
Specifically, it contains 2 lists: a list of Buy orders and a list of Sell orders, sorted in the order of decreasing priority.

For Buy order, a Buy order with a higher price has a higher priority than one with a lower price. If two buy orders have the same price, the one that arrived in the OB earlier will have higher priority.

For Sell order, a Sell order with a lower price has a higher priority than one with a lower price. If two Sell orders have the same price, the one that arrived in the OB earlier will have higher priority.

Each order is represented by a row of input. For example `SUB LO B Ffuj 200 13` 

1. SUB or CXL
2. LO (limit order) or MO (market order)
3. B (buy) or S(sell)
4. Order ID
5. Price
6. Quantity

When an order is submitted to the OB, it will get executed (sometimes partially) if there is a match with an order in the opposing Side within the OB. A match occurs between a Buy order and a Sell order if the price of the Buy order is greater than, or equal to, the price of the Sell order. The quantity matches (executed) is equal to the lower of the quantities of the two matched orders, and the price used in the trade will be the Price of the Order in the OB (not submitted order).

The machine engine will repeatedly match the submitted order with the order of highest priority in the opposing side within the OB and execute trades. If the limit order is not executed fully, it will be added to the OB with its remaining quantity. 

If it is a Market order, it will only consist of a Side and a Quantity but it will not have a Price. If the Market order is not executed fully, the remaining amount will be cancelled.

Apart from submitting orders, a trader can Cancel an order, hence removing from the OB. This will be indicated by the first item in a row, eg CXL.

###### Output
For each type of Order, output a single integer representing the total cost of the unit of stock that was traded during the execution of the order. It will be 0 if no stock was traded.

At the end of the sequence of actions, print out the entire OB in 2 lines. Each order will be represented in the format `[Quantity]@[Price]#[Order Id]`



##### Sample Inputs
###### Input 1

SUB LO B Ffuj 200 13 

SUB LO B Yy7P 150 11

SUB LO B YuFU 100 13

SUB LO S IpD8 150 14

SUB LO S y93N 190 15

SUB LO B Y5wb 230 14

SUB MO B IZLO 250 

CXL Ffuj

CXL 49Ze

END


###### Output 1
0

0

0

0

0

2100

2850

B: 80@14#Y5wb 100@13#YuFU 150@11#Yy7P

S: