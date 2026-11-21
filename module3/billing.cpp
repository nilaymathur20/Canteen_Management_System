#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int main() {

    string customerName;
    string itemName;
    int n, quantity;

    float price, amount;
    float subtotal = 0;
    float discountPercent;
    float discountAmount, finalAmount;

    cout << "\n";
    cout << "====================================\n";
    cout << "       CANTEEN MANAGEMENT SYSTEM\n";
    cout << "          MODULE 3 - BILLING\n";
    cout << "====================================\n";

    cout << "\nEnter Customer Name: ";
    getline(cin, customerName);

    cout << "\nEnter Number of Food Items: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of items!\n";
        return 0;
    }

    cout << "\n---------- FOOD DETAILS ----------\n";

    for (int i = 1; i <= n; i++) {

        cout << "\nEnter Food Item " << i << " Name: ";
        cin >> ws;
        getline(cin, itemName);

        cout << "Enter Price: Rs. ";
        cin >> price;

        cout << "Enter Quantity: ";
        cin >> quantity;

        if (price <= 0 || quantity <= 0) {
            cout << "Invalid price or quantity!\n";
            return 0;
        }

        amount = price * quantity;
        subtotal += amount;
    }

    cout << "\nEnter Discount Percentage: ";
    cin >> discountPercent;

    if (discountPercent < 0 || discountPercent > 100) {
        cout << "Invalid Discount!\n";
        return 0;
    }

    discountAmount = subtotal * discountPercent / 100;

    finalAmount = subtotal - discountAmount;

    cout << fixed << setprecision(2);

    cout << "\n";
    cout << "====================================\n";
    cout << "           CUSTOMER BILL\n";
    cout << "====================================\n";

    cout << "Customer Name : " << customerName << endl;

    cout << "Subtotal      : Rs. " << subtotal << endl;

    cout << "Discount      : " << discountPercent << "%" << endl;

    cout << "Discount Amt  : Rs. " << discountAmount << endl;

    cout << "------------------------------------\n";

    cout << "FINAL AMOUNT  : Rs. " << finalAmount << endl;

    cout << "====================================\n";
    cout << "       THANK YOU! VISIT AGAIN\n";
    cout << "====================================\n";

    return 0;
}
