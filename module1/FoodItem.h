
#ifndef FOODITEM_H
#define FOODITEM_H

#include <iostream>
#include <string>
using namespace std;

// Base Class
class FoodItem {
private:
    int id;
    string name;
    double price;
    string category;
    bool available;

    static int count;

public:

    // Default Constructor
    FoodItem();

    // Parameterized Constructor
    FoodItem(int i, string n, double p, string c, bool a = true);

    // Virtual Destructor
    virtual ~FoodItem();

    // Virtual Function
    virtual string typeNote() const;

    // Getters
    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    double getPrice() const {
        return price;
    }

    string getCategory() const {
        return category;
    }

    bool isAvailable() const {
        return available;
    }

    // Static Function
    static int getCount() {
        return count;
    }

    // Setters
    void setPrice(double p) {
        price = p;
    }

    void setAvailable(bool a) {
        available = a;
    }

    // File mein save karne ke liye
    string toFileLine() const;

    // Friend Functions
    friend ostream& operator<<(ostream &out, const FoodItem &f);

    friend bool compareByName(const FoodItem *a, const FoodItem *b);
    friend bool compareByPrice(const FoodItem *a, const FoodItem *b);
    friend bool compareByCategory(const FoodItem *a, const FoodItem *b);
};

// Friend function declarations
ostream& operator<<(ostream &out, const FoodItem &f);

bool compareByName(const FoodItem *a, const FoodItem *b);
bool compareByPrice(const FoodItem *a, const FoodItem *b);
bool compareByCategory(const FoodItem *a, const FoodItem *b);


// Derived Class 1
class Snack : public FoodItem {
public:
    Snack(int i, string n, double p, string c, bool a)
        : FoodItem(i, n, p, c, a) {}

    string typeNote() const override {
        return "Quick bite";
    }
};


// Derived Class 2
class Meal : public FoodItem {
public:
    Meal(int i, string n, double p, string c, bool a)
        : FoodItem(i, n, p, c, a) {}

    string typeNote() const override {
        return "Full meal";
    }
};


// Derived Class 3
class Drink : public FoodItem {
public:
    Drink(int i, string n, double p, string c, bool a)
        : FoodItem(i, n, p, c, a) {}

    string typeNote() const override {
        return "Beverage";
    }
};


// Category ke according object create karna
FoodItem* createItem(int id, string name, double price,
                     string category, bool available);

#endif
