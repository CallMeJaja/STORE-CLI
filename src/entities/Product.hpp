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
            const vector<string> &categories, int stock = 0, int sold = 0)
        : id(id), name(name), price(price), description(description),
          categories(categories), stock(stock), sold(sold) {}

    int getAvailableStock() const { return stock; }

    int getTotalQuantity() const { return stock + sold; }

    bool canPurchase(int quantity) const {
        return quantity > 0 && quantity <= stock;
    }

    bool validateStock() const { return stock >= 0; }

    bool validatePrice() const { return price > 0; }

    bool isLowStock(int threshold = 1) const { return stock <= threshold; }

    bool setPrice(int newPrice) {
        if (newPrice > 0) {
            price = newPrice;
            return true;
        }
        return false;
    }

    bool setStock(int newStock) {
        if (newStock >= 0) {
            stock = newStock;
            return true;
        }
        return false;
    }

    bool isAvailable() const { return stock > 0; }

    bool updateSold(int quantity) {
        if (quantity > 0 && quantity <= stock) {
            sold += quantity;
            stock -= quantity;
            return true;
        }
        return false;
    }

    bool isInStock() const { return stock > 0; }

    bool hasCategory(const string &category) {
        return find(categories.begin(), categories.end(), category) !=
               categories.end();
    }

    void restockProduct(int amount) {
        if (amount > 0) {
            stock += amount;
        }
    }

    void setName(const string &newName) { name = newName; }

    void setDescription(const string &newDescription) {
        description = newDescription;
    }

    void addCategory(const string &category) {
        if (find(categories.begin(), categories.end(), category) ==
            categories.end()) {
            categories.push_back(category);
        }
    }

    void removeCategory(const string &category) {
        categories.erase(remove(categories.begin(), categories.end(), category),
                         categories.end());
    }

    const vector<string> &getCategories() const { return categories; }
};
