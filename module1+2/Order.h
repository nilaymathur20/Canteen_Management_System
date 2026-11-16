#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include "Menu.h"

using namespace std;

class Order {
private:
    struct Line {
        int foodId;
        string foodName;
        double price;
        int quantity;
        double lineTotal;
    };

    int orderId;              // Order number (snapshot at creation)
    string customerName;      // Customer name
    vector<Line> lines;       // Ordered items (snapshot of price/name)
    double total;             // Snapshot at creation
    string status;            // "Pending" | "Completed" | "Cancelled"
    string timestamp;         // "YYYY-MM-DD HH:MM"
    static const string ORDERS_FILE;

    // ---- File persistence (orders.txt) ----
    static void saveOrder(const Order& o);          // rewrite whole file
    static void saveAll(const vector<Order>& all);  // rewrite whole file from list
    static vector<Order> loadAll();                 // read file into memory
    static Order findOrder(int id);                 // null-order if not found

    // ---- Receipt printing ----
    void printReceiptLine() const;
    void printReceipt() const;

public:
    Order();
    ~Order();

    // Factory
    static int nextId();

    // ---- Order management (connected to Menu) ----
    static void placeOrder(Menu& menu);       // place + mark sold out + save
    static void viewOrders();                 // list all orders
    static void viewReceipt(int orderId);     // print itemized receipt
    static void cancelOrder(int orderId, Menu& menu);     // cancel + (optionally) re-stock

    // ---- Building the order (called from placeOrder) ----
    void addLine(int foodId, const string& foodName, double price, int qty);
    void finalize(Menu& menu);                // save + mark sold out + receipt
};

#endif
