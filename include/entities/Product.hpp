#pragma once
#include "string"
#include "vector"
#include <algorithm>

using namespace std;

class Product {
  public:
    int id;
    int price;
    int stock;
    int sold;
    string name;
    string description;
    vector<string> categories;

    Product(int id, const string &name, int price, const string &description,
            const vector<string> &categories, int stock = 0, int sold = 0);

    int getTotalQuantity();
    bool canPurchase(int quantity);
    bool validateStock();
    bool isLowStock(int threshold = 1);

    bool setPrice(int newPrice);
    bool setStock(int newStock);

    bool isAvailable();
    bool updateSold(int quantity);

    bool isInStock();
    bool hasCategory(const string &category);
    void restockProduct(int amount);

    void setName(const string &newName);
    void setDescription(const string &newDescription);

    void addCategory(const string &category);
    void removeCategory(const string &category);
    const vector<string> &getCategories();
};