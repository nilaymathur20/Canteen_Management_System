
#include <iostream>
#include "Menu.h"
#include "Helper.h"

using namespace std;


// Main Application Class
class CanteenApp {

private:

    // Menu class ka object
    Menu menu;


    // Menu Management
    void menuModule() {

        int choice;

        do {

            cout << "\n===== MENU MANAGEMENT =====\n";

            cout << "1. Add Food Item\n";
            cout << "2. View Menu\n";
            cout << "3. Update Price\n";
            cout << "4. Edit Item\n";
            cout << "5. Delete Item\n";
            cout << "6. Search Item\n";
            cout << "7. Category Wise Menu\n";
            cout << "8. Available / Sold Out\n";
            cout << "9. Sorted Menu\n";
            cout << "0. Back\n";

            choice = (int)readNumber("Enter Choice: ");

            switch (choice) {

                case 1:
                    menu.addItem();
                    break;

                case 2:
                    menu.viewMenu();
                    break;

                case 3:
                    menu.updatePrice();
                    break;

                case 4:
                    menu.editItem();
                    break;

                case 5:
                    menu.deleteItem();
                    break;

                case 6:
                    menu.searchItem();
                    break;

                case 7:
                    menu.viewByCategory();
                    break;

                case 8:
                    menu.toggleAvailability();
                    break;

                case 9:
                    menu.viewSorted();
                    break;

                case 0:
                    cout << "Going Back...\n";
                    break;

                default:
                    cout << "Invalid Choice!\n";
            }

        } while (choice != 0);
    }


public:

    // Main Application Function
    void run() {

        int choice;

        do {

            cout << "\n";
            cout << "===============================\n";
            cout << "   CANTEEN MANAGEMENT SYSTEM\n";
            cout << "===============================\n";

            cout << "1. Manage Menu & Prices\n";
            cout << "2. Take Orders (Coming Soon)\n";
            cout << "3. Bills & Discounts (Coming Soon)\n";
            cout << "4. Daily Sales Report (Coming Soon)\n";
            cout << "5. Exit\n";

            choice = (int)readNumber("Enter Choice: ");

            switch (choice) {

                case 1:
                    menuModule();
                    break;

                case 2:
                case 3:
                case 4:
                    cout << "This Module is Coming Soon!\n";
                    break;

                case 5:
                    cout << "Thank You!\n";
                    break;

                default:
                    cout << "Invalid Choice!\n";
            }

        } while (choice != 5);
    }
};


// Main Function
int main() {

    CanteenApp app;

    app.run();

    return 0;
}
