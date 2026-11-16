#include "Order.h"
#include "Helper.h"

// -----------------------------------------------------------
// Static state
// -----------------------------------------------------------
const string Order::ORDERS_FILE = "orders.txt";

// -----------------------------------------------------------
// Factory
// -----------------------------------------------------------
int Order::nextId() {
    int maxId = 0;
    ifstream file(ORDERS_FILE);
    if (!file.is_open()) return 1;

    string line;
    while (getline(file, line)) {
        if (line.compare(0, 4, "ORD|") == 0) {
            size_t start = 4;
            size_t end = line.find('|', start);
            if (end == string::npos) continue;
            try {
                int id = stoi(line.substr(start, end - start));
                if (id > maxId) maxId = id;
            } catch (...) {
                // skip malformed header line
            }
        }
    }
    file.close();
    return maxId + 1;   // unique, persists across program restarts
}

// -----------------------------------------------------------
// Persistence helpers
// -----------------------------------------------------------
void Order::saveAll(const vector<Order>& all) {
    ofstream file(ORDERS_FILE);
    if (!file.is_open()) {
        cout << "Error: could not open " << ORDERS_FILE << " for writing!\n";
        return;
    }
    for (const auto& o : all) {
        file << "ORD|" << o.orderId << "|" << o.customerName << "|"
             << o.status << "|" << fixed << setprecision(2) << o.total << "|"
             << o.timestamp << "\n";
        for (const auto& l : o.lines) {
            file << "ITEM|" << l.foodId << "|" << l.foodName << "|"
                 << fixed << setprecision(2) << l.price << "|"
                 << l.quantity << "\n";
        }
        file << "\n";   // order separator
    }
    file.close();
}

void Order::saveOrder(const Order& o) {
    vector<Order> all = loadAll();
    all.erase(remove_if(all.begin(), all.end(),
                        [&](const Order& x) { return x.orderId == o.orderId; }),
              all.end());
    all.push_back(o);
    saveAll(all);
}

vector<Order> Order::loadAll() {
    vector<Order> orders;
    Order* cur = nullptr;

    ifstream file(ORDERS_FILE);
    if (!file.is_open()) {
        return orders;   // file missing => no orders yet
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            if (cur) {
                orders.push_back(*cur);
                delete cur;
                cur = nullptr;
            }
            continue;
        }
        if (line.compare(0, 4, "ORD|") == 0) {
            cur = new Order();
            stringstream ss(line.substr(4));
            string tid, name, status, totalStr, ts;
            getline(ss, tid, '|');
            getline(ss, name, '|');
            getline(ss, status, '|');
            getline(ss, totalStr, '|');
            getline(ss, ts, '|');
            try {
                cur->orderId = stoi(tid);
                cur->customerName = name;
                cur->status = status;
                cur->total = stod(totalStr);
                cur->timestamp = ts;
            } catch (...) {
                delete cur;
                cur = nullptr;
            }
        } else if (line.compare(0, 5, "ITEM|") == 0 && cur) {
            stringstream ss(line.substr(5));
            string fid, name, price, qty;
            getline(ss, fid, '|');
            getline(ss, name, '|');
            getline(ss, price, '|');
            getline(ss, qty, '|');
            try {
                Line l;
                l.foodId = stoi(fid);
                l.foodName = name;
                l.price = stod(price);
                l.quantity = stoi(qty);
                l.lineTotal = l.price * l.quantity;
                cur->lines.push_back(l);
            } catch (...) {
                // skip malformed item line
            }
        }
    }
    if (cur) {
        orders.push_back(*cur);
        delete cur;
    }
    file.close();
    return orders;
}

Order Order::findOrder(int id) {
    for (auto& o : loadAll()) {
        if (o.orderId == id) return o;
    }
    Order none;
    none.orderId = -1;
    return none;
}

// -----------------------------------------------------------
// Order lifecycle
// -----------------------------------------------------------
Order::Order() {
    orderId = nextId();
    customerName = "";
    status = "Pending";
    timestamp = "";
    total = 0.0;
}

Order::~Order() {
    // no heap resources owned
}

void Order::addLine(int foodId, const string& foodName, double price, int qty) {
    Line l;
    l.foodId = foodId;
    l.foodName = foodName;
    l.price = price;
    l.quantity = qty;
    l.lineTotal = price * qty;
    lines.push_back(l);
    total += l.lineTotal;
}

// Build receipt + mark sold out when the order is placed
void Order::finalize(Menu& menu) {
    saveOrder(*this);
    cout << "\n---------------- ORDER PLACED ----------------\n";
    printReceipt();
    cout << "---------------------------------------------\n";

    // Mark every ordered item as Sold Out in the menu
    for (const auto& l : lines) {
        menu.markSoldOut(l.foodId);
    }
}

// -----------------------------------------------------------
// Interactive: place order
// -----------------------------------------------------------
void Order::placeOrder(Menu& menu) {
    vector<FoodItem*> avail = menu.getAvailableItems();
    if (avail.empty()) {
        cout << "No items available to order right now.\n";
        return;
    }

    cout << "\n===== TAKE ORDER =====\n";

    Order order;
    order.customerName = readText("Customer Name: ");

    int choice = 1;
    while (choice != 0) {
        cout << "\n--- Add item (pick from the available list below) ---\n";
        avail = menu.getAvailableItems();
        if (avail.empty()) {
            cout << "No more items available.\n";
            break;
        }
        menu.printTable(avail);

        int foodId = (int)readNumber("Food ID (0 to finish): ");
        if (foodId == 0) break;

        FoodItem* item = nullptr;
        for (auto* f : avail) {
            if (f->getId() == foodId) {
                item = f;
                break;
            }
        }
        if (!item) {
            cout << "Item not found or not available.\n";
            continue;
        }

        int qty = (int)readNumber("Quantity: ");
        if (qty <= 0) {
            cout << "Invalid quantity.\n";
            continue;
        }
        order.addLine(foodId, item->getName(), item->getPrice(), qty);
        cout << "  Added: " << item->getName() << " x" << qty
             << " = " << fixed << setprecision(2) << item->getPrice() * qty << "\n";
    }

    if (order.lines.empty()) {
        cout << "No items added. Order cancelled.\n";
        return;
    }

    cout << "\nOrder Summary:\n";
    order.printReceipt();
    cout << "Total: " << fixed << setprecision(2) << order.total << "\n";

    string ans = toLower(readText("Place order? (y/n): "));
    if (toLower(ans) != "y") {
        cout << "Order cancelled.\n";
        return;
    }

    // Timestamp the order
    time_t now = time(0);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&now));
    order.timestamp = buf;

    order.finalize(menu);
}

// Print one receipt line
void Order::printReceiptLine() const {
    for (const auto& l : lines) {
        cout << setw(5) << l.foodId
             << setw(20) << l.foodName
             << setw(8) << l.quantity
             << setw(10) << fixed << setprecision(2) << l.price
             << setw(10) << l.lineTotal << "\n";
    }
}

void Order::printReceipt() const {
    cout << "\n";
    cout << setw(32) << "CANTEEN ORDER RECEIPT" << "\n";
    cout << setw(22) << "Order #" << setw(6) << orderId << "\n";
    cout << setw(22) << "Customer" << setw(6) << customerName << "\n";
    cout << setw(22) << "Date/Time" << setw(6) << timestamp << "\n";
    cout << "----------------------------------------\n";
    if (lines.empty()) {
        cout << "No items\n";
    } else {
        cout << left << setw(5) << "ID"
             << setw(20) << "Item"
             << setw(8) << "Qty"
             << setw(10) << "Price"
             << setw(10) << "Total" << "\n";
        cout << "----------------------------------------\n";
        printReceiptLine();
    }
    cout << "----------------------------------------\n";
    cout << setw(34) << "TOTAL" << setw(10) << fixed << setprecision(2) << total << "\n";
    cout << "----------------------------------------\n";
    cout << "Status: " << status << "\n";
    cout << "\n";
}

// -----------------------------------------------------------
// List all orders
// -----------------------------------------------------------
void Order::viewOrders() {
    vector<Order> all = loadAll();
    if (all.empty()) {
        cout << "No orders yet.\n";
        return;
    }
    cout << "\n===== ORDERS =====\n";
    cout << left << setw(6) << "ID"
         << setw(20) << "Customer"
         << setw(12) << "Status"
         << setw(16) << "Total"
         << setw(16) << "Date" << "\n";
    cout << string(64, '-') << "\n";
    for (const auto& o : all) {
        cout << left << setw(6) << o.orderId
             << setw(20) << o.customerName
             << setw(12) << o.status
             << setw(16) << fixed << setprecision(2) << o.total
             << setw(16) << o.timestamp.substr(0, 10) << "\n";
    }
    cout << "================\n";
}

// -----------------------------------------------------------
// Print receipt for one order
// -----------------------------------------------------------
void Order::viewReceipt(int orderId) {
    Order o = findOrder(orderId);
    if (o.orderId == -1) {
        cout << "Order not found.\n";
        return;
    }
    o.printReceipt();
}

// -----------------------------------------------------------
// Cancel an order + optional re-stock
// -----------------------------------------------------------
void Order::cancelOrder(int orderId, Menu& menu) {
    Order o = findOrder(orderId);
    if (o.orderId == -1) {
        cout << "Order not found.\n";
        return;
    }
    if (o.status == "Cancelled") {
        cout << "Order is already cancelled.\n";
        return;
    }
    if (o.status == "Completed") {
        cout << "Order already completed; cannot cancel.\n";
        return;
    }

    cout << "Order " << orderId << " will be cancelled.\n";
    string ans = toLower(readText("Re-stock items and mark them Available again? (y/n): "));
    if (toLower(ans) == "y") {
        for (const auto& l : o.lines) {
            menu.markAvailable(l.foodId);
        }
    }

    // Rewrite file with this order marked Cancelled
    vector<Order> all = loadAll();
    for (auto& o2 : all) {
        if (o2.orderId == orderId) {
            o2.status = "Cancelled";
            break;
        }
    }
    saveAll(all);
    cout << "Order " << orderId << " cancelled.\n";
}
