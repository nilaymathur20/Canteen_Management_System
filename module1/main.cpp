
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
using namespace std;

// ================= HELPER FUNCTIONS =================

string lowerCase(string s) {
    for (int i = 0; i < s.length(); i++)
        s[i] = tolower(s[i]);
    return s;
}

// ================= BASE CLASS =================

class FoodItem {
private:
    int id;
    string name;
    double price;
    string category;
    bool available;

    static int count;

public:
    FoodItem(int i, string n, double p, string c, bool a = true) {
        id = i;
        name = n;
        price = p;
        category = c;
        available = a;
        count++;
    }

    virtual ~FoodItem() {
        count--;
    }

    int getId() { return id; }
    string getName() { return name; }
    double getPrice() { return price; }
    string getCategory() { return category; }
    bool isAvailable() { return available; }

    void setPrice(double p) { price = p; }
    void setName(string n) { name = n; }
    void setCategory(string c) { category = c; }
    void setAvailable(bool a) { available = a; }

    static int getCount() {
        return count;
    }

    virtual string getType() {
        return "Food Item";
    }

    friend ostream& operator<<(ostream& out, FoodItem& f) {
        out << left << setw(5) << f.id
            << setw(18) << f.name
            << setw(10) << f.price
            << setw(12) << f.category
            << setw(12) << (f.available ? "Available" : "Sold Out")
            << f.getType();
        return out;
    }

    string saveData() {
        return to_string(id) + "|" + name + "|" +
               to_string(price) + "|" + category + "|" +
               to_string(available);
    }
};

int FoodItem::count = 0;

// ================= INHERITANCE =================

class Snack : public FoodItem {
public:
    Snack(int i, string n, double p, string c, bool a)
        : FoodItem(i, n, p, c, a) {}

    string getType() override {
        return "Snack";
    }
};

class Meal : public FoodItem {
public:
    Meal(int i, string n, double p, string c, bool a)
        : FoodItem(i, n, p, c, a) {}

    string getType() override {
        return "Meal";
    }
};

class Drink : public FoodItem {
public:
    Drink(int i, string n, double p, string c, bool a)
        : FoodItem(i, n, p, c, a) {}

    string getType() override {
        return "Drink";
    }
};

// ================= MENU CLASS =================

class Menu {
private:
    vector<FoodItem*> items;

    FoodItem* createItem(int id, string name,
                         double price, string category,
                         bool available) {

        if (lowerCase(category) == "snacks")
            return new Snack(id, name, price, category, available);

        else if (lowerCase(category) == "meals")
            return new Meal(id, name, price, category, available);

        else if (lowerCase(category) == "drinks")
            return new Drink(id, name, price, category, available);

        return new FoodItem(id, name, price, category, available);
    }

    int findItem(int id) {
        for (int i = 0; i < items.size(); i++) {
            if (items[i]->getId() == id)
                return i;
        }
        return -1;
    }

    void display(vector<FoodItem*> list) {
        cout << "\n";
        cout << left << setw(5) << "ID"
             << setw(18) << "Name"
             << setw(10) << "Price"
             << setw(12) << "Category"
             << setw(12) << "Status"
             << "Type\n";

        cout << "-------------------------------------------------------------\n";

        for (int i = 0; i < list.size(); i++)
            cout << *list[i] << endl;
    }

    void save() {
        ofstream file("menu.txt");

        for (int i = 0; i < items.size(); i++)
            file << items[i]->saveData() << endl;

        file.close();
    }

    void load() {
        ifstream file("menu.txt");
        string line;

        while (getline(file, line)) {
            int id, available;
            double price;
            string name, category;

            size_t p1 = line.find('|');
            size_t p2 = line.find('|', p1 + 1);
            size_t p3 = line.find('|', p2 + 1);
            size_t p4 = line.find('|', p3 + 1);

            if (p1 == string::npos || p2 == string::npos ||
                p3 == string::npos || p4 == string::npos)
                continue;

            try {
                id = stoi(line.substr(0, p1));
                name = line.substr(p1 + 1, p2 - p1 - 1);
                price = stod(line.substr(p2 + 1, p3 - p2 - 1));
                category = line.substr(p3 + 1, p4 - p3 - 1);
                available = stoi(line.substr(p4 + 1));

                if (findItem(id) == -1 && id > 0 && price > 0) {
                    items.push_back(createItem(
                        id, name, price, category, available
                    ));
                }
            }
            catch (...) {
                continue;
            }
        }

        file.close();
    }

public:

    Menu() {
        load();
    }

    ~Menu() {
        for (int i = 0; i < items.size(); i++)
            delete items[i];
    }

    // 1. ADD ITEM

    void addItem() {
        string name, category;
        double price;

        cout << "Enter Item Name: ";
        getline(cin, name);

        cout << "Enter Price: ";
        cin >> price;
        cin.ignore();

        if (name.empty() || price <= 0) {
            cout << "Invalid Details!\n";
            return;
        }

        cout << "Enter Category (Snacks/Meals/Drinks): ";
        getline(cin, category);

        if (category.empty() || name.find('|') != string::npos ||
            category.find('|') != string::npos) {
            cout << "Invalid Details!\n";
            return;
        }

        int id = 1;

        for (int i = 0; i < items.size(); i++) {
            if (items[i]->getId() >= id)
                id = items[i]->getId() + 1;
        }

        items.push_back(createItem(id, name, price, category, true));

        save();
        cout << "Item Added Successfully!\n";
    }

    // 2. VIEW MENU

    void viewMenu() {
        if (items.empty()) {
            cout << "Menu is Empty!\n";
            return;
        }

        display(items);
        cout << "\nTotal Items: " << items.size();
        cout << "\nTotal Objects: " << FoodItem::getCount() << endl;
    }

    // 3. UPDATE PRICE

    void updatePrice() {
        int id;
        double price;

        viewMenu();

        cout << "\nEnter Item ID: ";
        cin >> id;

        int index = findItem(id);

        if (index == -1) {
            cout << "Item Not Found!\n";
            return;
        }

        cout << "Enter New Price: ";
        cin >> price;

        if (price <= 0) {
            cout << "Invalid Price!\n";
            return;
        }

        items[index]->setPrice(price);
        save();

        cout << "Price Updated!\n";
    }

    // 4. EDIT ITEM

    void editItem() {
        int id;
        string name, category;

        viewMenu();

        cout << "\nEnter Item ID: ";
        cin >> id;
        cin.ignore();

        int index = findItem(id);

        if (index == -1) {
            cout << "Item Not Found!\n";
            return;
        }

        cout << "Enter New Name: ";
        getline(cin, name);

        cout << "Enter New Category: ";
        getline(cin, category);

        if (!name.empty() && name.find('|') == string::npos)
            items[index]->setName(name);

        if (!category.empty() && category.find('|') == string::npos) {
            FoodItem* newItem = createItem(
                id,
                items[index]->getName(),
                items[index]->getPrice(),
                category,
                items[index]->isAvailable()
            );

            delete items[index];
            items[index] = newItem;
        }

        save();
        cout << "Item Updated!\n";
    }

    // 5. DELETE ITEM

    void deleteItem() {
        int id;

        viewMenu();

        cout << "\nEnter Item ID: ";
        cin >> id;

        int index = findItem(id);

        if (index == -1) {
            cout << "Item Not Found!\n";
            return;
        }

        delete items[index];
        items.erase(items.begin() + index);

        save();
        cout << "Item Deleted!\n";
    }

    // 6. SEARCH ITEM

    void searchItem() {
        string name;

        cout << "Enter Item Name: ";
        getline(cin, name);

        vector<FoodItem*> result;

        for (int i = 0; i < items.size(); i++) {
            if (lowerCase(items[i]->getName()).find(lowerCase(name))
                != string::npos) {
                result.push_back(items[i]);
            }
        }

        if (result.empty())
            cout << "Item Not Found!\n";
        else
            display(result);
    }

    // 7. CATEGORY WISE MENU

    void categoryMenu() {
        string category;

        cout << "Enter Category: ";
        getline(cin, category);

        vector<FoodItem*> result;

        for (int i = 0; i < items.size(); i++) {
            if (lowerCase(items[i]->getCategory()) == lowerCase(category))
                result.push_back(items[i]);
        }

        if (result.empty())
            cout << "No Items Found!\n";
        else
            display(result);
    }

    // 8. AVAILABLE / SOLD OUT

    void availability() {
        int id;

        viewMenu();

        cout << "\nEnter Item ID: ";
        cin >> id;

        int index = findItem(id);

        if (index == -1) {
            cout << "Item Not Found!\n";
            return;
        }

        items[index]->setAvailable(!items[index]->isAvailable());

        save();
        cout << "Availability Updated!\n";
    }

    // 9. SORTING

    void sortMenu() {
        int choice;

        cout << "\n1. Sort By Name";
        cout << "\n2. Sort By Price";
        cout << "\n3. Sort By Category";
        cout << "\nEnter Choice: ";
        cin >> choice;

        vector<FoodItem*> temp = items;

        if (choice == 1) {
            sort(temp.begin(), temp.end(),
                [](FoodItem* a, FoodItem* b) {
                    return lowerCase(a->getName()) <
                           lowerCase(b->getName());
                });
        }
        else if (choice == 2) {
            sort(temp.begin(), temp.end(),
                [](FoodItem* a, FoodItem* b) {
                    return a->getPrice() < b->getPrice();
                });
        }
        else if (choice == 3) {
            sort(temp.begin(), temp.end(),
                [](FoodItem* a, FoodItem* b) {
                    return a->getCategory() < b->getCategory();
                });
        }
        else {
            cout << "Invalid Choice!\n";
            return;
        }

        display(temp);
    }

    // MENU MANAGEMENT

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

            cout << "Enter Choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: addItem(); break;
                case 2: viewMenu(); break;
                case 3: updatePrice(); break;
                case 4: editItem(); break;
                case 5: deleteItem(); break;
                case 6: searchItem(); break;
                case 7: categoryMenu(); break;
                case 8: availability(); break;
                case 9: sortMenu(); break;
                case 0: cout << "Going Back!\n"; break;
                default: cout << "Invalid Choice!\n";
            }

        } while (choice != 0);
    }
};

// ================= MAIN APPLICATION =================

class CanteenApp {
private:
    Menu menu;

public:
    void run() {
        int choice;

        do {
            cout << "\n==============================";
            cout << "\n   CANTEEN MANAGEMENT SYSTEM";
            cout << "\n==============================\n";

            cout << "1. Manage Menu & Prices\n";
            cout << "2. Take Orders (Coming Soon)\n";
            cout << "3. Bills & Discounts (Coming Soon)\n";
            cout << "4. Daily Sales Report (Coming Soon)\n";
            cout << "5. Exit\n";

            cout << "Enter Choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    menu.menuModule();
                    break;

                case 2:
                case 3:
                case 4:
                    cout << "Module Coming Soon!\n";
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

// ================= MAIN FUNCTION =================

int main() {
    CanteenApp app;
    app.run();

    return 0;
}
