
#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <vector>
#include <string>
#include "FoodItem.h"

using namespace std;

class Menu {

private:

    // Food items store karne ke liye
    vector<FoodItem*> items;

    // File ka naam
    string fileName;

    // Copy constructor and assignment private
    Menu(const Menu&);
    Menu& operator=(const Menu&);

    friend class Order;

    // Helper functions
    int findIndex(int id) const;

    bool nameExists(string name, int ignoreId = -1) const;

    int askForIndex(string message) const;

    void printTable(const vector<FoodItem*>& list) const;


public:

    // Constructor
    Menu(string file = "menu.txt");

    // Destructor
    ~Menu();


    // File Handling
    void load();

    void save() const;


    // Feature 1: Add Item
    void addItem();

    // Feature 2: View Menu
    void viewMenu() const;

    // Feature 3: Update Price
    void updatePrice();

    // Feature 4: Edit Item
    void editItem();

    // Feature 5: Delete Item
    void deleteItem();

    // Feature 6: Search Item
    void searchItem() const;

    // Feature 7: Category Wise Menu
    void viewByCategory() const;

    // Feature 8: Availability Change
    void toggleAvailability();

    // Feature 9: Sorted Menu
    void viewSorted() const;


    // Available items for future order module
    vector<FoodItem*> getAvailableItems() const;

    // Mark an item as Sold Out (used by the orders module)
    void markSoldOut(int id);

    // Mark an item as Available again (used by the orders module)
    void markAvailable(int id);

};

#endif
