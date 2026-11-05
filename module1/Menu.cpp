#include "Menu.h"
#include "Helper.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>


// Constructor
Menu::Menu(string file) {
    fileName = file;
    load();
}


// Destructor
Menu::~Menu() {
    for (int i = 0; i < items.size(); i++) {
        delete items[i];
    }
}


// File se data load karna
void Menu::load() {

    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Warning: " << fileName << " nahi mili. Nayi file ban jayegi.\n";
        return;
    }

    string line;

    while (getline(file, line)) {

        // Windows ka extra '\r' hatao
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }

        if (line.empty()) continue;

        stringstream ss(line);

        string id, name, price, category, available;

        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, price, '|');
        getline(ss, category, '|');
        getline(ss, available, '|');

        try {
            items.push_back(
                createItem(stoi(id), name, stod(price),
                           category, available != "0")
            );
        }
        catch (...) {
            // Invalid line ko ignore karo
        }
    }

    file.close();
}


// File mein data save karna
void Menu::save() const {

    ofstream file(fileName);

    for (int i = 0; i < items.size(); i++) {
        file << items[i]->toFileLine() << endl;
    }

    file.close();
}


// ID se item find karna
int Menu::findIndex(int id) const {

    for (int i = 0; i < items.size(); i++) {

        if (items[i]->getId() == id) {
            return i;
        }
    }

    return -1;
}


// Duplicate name check
bool Menu::nameExists(string name, int ignoreId) const {

    for (int i = 0; i < items.size(); i++) {

        if (items[i]->getId() != ignoreId &&
            toLower(items[i]->getName()) == toLower(name)) {

            return true;
        }
    }

    return false;
}


// ID input lena
int Menu::askForIndex(string message) const {

    int id = (int)readNumber(message);

    int index = findIndex(id);

    if (index == -1) {
        cout << "Item nahi mila!\n";
    }

    return index;
}


// Table print karna
void Menu::printTable(const vector<FoodItem*>& list) const {

    cout << "\n";
    cout << "---------------------------------------------------------------\n";

    cout << left
         << setw(5) << "ID"
         << setw(20) << "Name"
         << setw(10) << "Price"
         << setw(12) << "Category"
         << setw(12) << "Status"
         << "Type\n";

    cout << "---------------------------------------------------------------\n";

    for (int i = 0; i < list.size(); i++) {
        cout << *list[i] << endl;
    }

    cout << "---------------------------------------------------------------\n";
}


// 1. Add Item
void Menu::addItem() {

    string name = readText("Item Name: ");

    if (name.empty() || name.find('|') != string::npos) {
        cout << "Invalid name!\n";
        return;
    }

    if (nameExists(name)) {
        cout << "Item already exists!\n";
        return;
    }

    double price = readNumber("Enter Price: ");

    if (price <= 0) {
        cout << "Invalid price!\n";
        return;
    }

    string category = readText("Category (Snacks/Meals/Drinks): ");

    if (category.empty() || category.find('|') != string::npos) {
        cout << "Invalid category!\n";
        return;
    }

    category = capitalize(category);

    // Sabse bada ID dhoondo, usse +1 karo (duplicate ID nahi banegi)
    int id = 1;

    for (int i = 0; i < items.size(); i++) {
        if (items[i]->getId() >= id) {
            id = items[i]->getId() + 1;
        }
    }

    items.push_back(
        createItem(id, name, price, category, true)
    );

    save();

    cout << "Item added successfully!\n";
}


// 2. View Menu
void Menu::viewMenu() const {

    if (items.empty()) {
        cout << "Menu is empty!\n";
        return;
    }

    printTable(items);

    cout << "Total Items: " << items.size() << endl;
}


// 3. Update Price
void Menu::updatePrice() {

    if (items.empty()) {
        cout << "Menu is empty!\n";
        return;
    }

    printTable(items);

    int index = askForIndex("Enter Item ID: ");

    if (index == -1) return;

    double price = readNumber("Enter New Price: ");

    if (price <= 0) {
        cout << "Invalid price!\n";
        return;
    }

    items[index]->setPrice(price);

    save();

    cout << "Price updated successfully!\n";
}


// 4. Edit Item
void Menu::editItem() {

    if (items.empty()) {
        cout << "Menu is empty!\n";
        return;
    }

    printTable(items);

    int index = askForIndex("Enter Item ID: ");

    if (index == -1) return;

    string name = readText("Enter New Name (Enter to skip): ");

    if (!name.empty()) {

        if (name.find('|') != string::npos ||
            nameExists(name, items[index]->getId())) {

            cout << "Invalid or duplicate name!\n";
            return;
        }
    }
    else {
        name = items[index]->getName();
    }

    string category = readText("Enter New Category (Enter to skip): ");

    if (!category.empty()) {

        if (category.find('|') != string::npos) {
            cout << "Invalid category!\n";
            return;
        }

        category = capitalize(category);
    }
    else {
        category = items[index]->getCategory();
    }

    FoodItem* newItem = createItem(
        items[index]->getId(),
        name,
        items[index]->getPrice(),
        category,
        items[index]->isAvailable()
    );

    delete items[index];

    items[index] = newItem;

    save();

    cout << "Item updated successfully!\n";
}


// 5. Delete Item
void Menu::deleteItem() {

    if (items.empty()) {
        cout << "Menu is empty!\n";
        return;
    }

    printTable(items);

    int index = askForIndex("Enter Item ID: ");

    if (index == -1) return;

    string answer = readText("Are you sure? (y/n): ");

    if (toLower(answer) != "y") {
        cout << "Delete cancelled!\n";
        return;
    }

    delete items[index];

    items.erase(items.begin() + index);

    save();

    cout << "Item deleted successfully!\n";
}


// 6. Search Item
void Menu::searchItem() const {

    string name = toLower(readText("Enter item name: "));

    vector<FoodItem*> result;

    for (int i = 0; i < items.size(); i++) {

        if (toLower(items[i]->getName()).find(name) != string::npos) {
            result.push_back(items[i]);
        }
    }

    if (result.empty()) {
        cout << "Item not found!\n";
    }
    else {
        printTable(result);
    }
}


// 7. Category Wise Menu
void Menu::viewByCategory() const {

    string category = toLower(readText("Enter Category: "));

    vector<FoodItem*> result;

    for (int i = 0; i < items.size(); i++) {

        if (toLower(items[i]->getCategory()) == category) {
            result.push_back(items[i]);
        }
    }

    if (result.empty()) {
        cout << "No item found!\n";
    }
    else {
        printTable(result);
    }
}


// 8. Available / Sold Out
void Menu::toggleAvailability() {

    if (items.empty()) {
        cout << "Menu is empty!\n";
        return;
    }

    printTable(items);

    int index = askForIndex("Enter Item ID: ");

    if (index == -1) return;

    bool status = !items[index]->isAvailable();

    items[index]->setAvailable(status);

    save();

    cout << "Status updated successfully!\n";
}


// 9. Sorted Menu
void Menu::viewSorted() const {

    if (items.empty()) {
        cout << "Menu is empty!\n";
        return;
    }

    cout << "\n1. Sort by Name";
    cout << "\n2. Sort by Price";
    cout << "\n3. Sort by Category\n";

    int choice = (int)readNumber("Enter Choice: ");

    vector<FoodItem*> temp = items;

    if (choice == 1) {
        sort(temp.begin(), temp.end(), compareByName);
    }
    else if (choice == 2) {
        sort(temp.begin(), temp.end(), compareByPrice);
    }
    else if (choice == 3) {
        sort(temp.begin(), temp.end(), compareByCategory);
    }
    else {
        cout << "Invalid choice!\n";
        return;
    }

    printTable(temp);
}


// Available items return karna
vector<FoodItem*> Menu::getAvailableItems() const {

    vector<FoodItem*> result;

    for (int i = 0; i < items.size(); i++) {

        if (items[i]->isAvailable()) {
            result.push_back(items[i]);
        }
    }

    return result;
}