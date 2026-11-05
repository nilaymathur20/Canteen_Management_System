
#include "FoodItem.h"
#include "Helper.h"
#include <sstream>
#include <iomanip>

// Static variable
int FoodItem::count = 0;


// Default Constructor
FoodItem::FoodItem() {
    id = 0;
    name = "";
    price = 0;
    category = "";
    available = true;

    count++;
}


// Parameterized Constructor
FoodItem::FoodItem(int i, string n, double p, string c, bool a) {
    id = i;
    name = n;
    price = p;
    category = c;
    available = a;

    count++;
}


// Destructor
FoodItem::~FoodItem() {
    count--;
}


// Virtual Function
string FoodItem::typeNote() const {
    return "General item";
}


// File ke liye data convert karna
string FoodItem::toFileLine() const {
    stringstream ss;

    ss << id << "|"
       << name << "|"
       << price << "|"
       << category << "|"
       << available;

    return ss.str();
}


// Friend Function: Display Food Item
ostream& operator<<(ostream &out, const FoodItem &f) {

    out << left
        << setw(5) << f.id
        << setw(20) << f.name
        << setw(10) << f.price
        << setw(12) << f.category
        << setw(12) << (f.available ? "Available" : "Sold Out")
        << f.typeNote();

    return out;
}


// Sort by Name
bool compareByName(const FoodItem *a, const FoodItem *b) {
    return toLower(a->getName()) < toLower(b->getName());
}


// Sort by Price
bool compareByPrice(const FoodItem *a, const FoodItem *b) {
    return a->getPrice() < b->getPrice();
}


// Sort by Category
bool compareByCategory(const FoodItem *a, const FoodItem *b) {
    return a->getCategory() < b->getCategory();
}


// Create object according to category
FoodItem* createItem(int id, string name, double price,
                     string category, bool available) {

    string c = toLower(category);

    if (c == "snacks") {
        return new Snack(id, name, price, category, available);
    }

    else if (c == "meals") {
        return new Meal(id, name, price, category, available);
    }

    else if (c == "drinks") {
        return new Drink(id, name, price, category, available);
    }

    else {
        return new FoodItem(id, name, price, category, available);
    }
}
