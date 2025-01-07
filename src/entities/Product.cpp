#include "entities/Product.hpp"

Product::Product(int id, const string &name, int price,
                 const string &description, const vector<string> &categories,
                 int stock, int sold)
    : id(id), name(name), price(price), description(description),
      categories(categories), stock(stock), sold(sold) {}

int Product::getTotalQuantity() { return stock; }

bool Product::canPurchase(int quantity) { return stock >= quantity; }

bool Product::validateStock() { return stock >= 0; }

bool Product::isLowStock(int threshold) { return stock < threshold; }
bool Product::setPrice(int newPrice) {
    if (newPrice > 0) {
        price = newPrice;
        return true;
    }
    return false;
}

bool Product::setStock(int newStock) {
    if (newStock >= 0) {
        stock = newStock;
        return true;
    }
    return false;
}

bool Product::isAvailable() { return stock > 0; }

bool Product::updateSold(int quantity) {
    if (quantity > 0 && stock >= quantity) {
        sold += quantity;
        stock -= quantity;
        return true;
    }
    return false;
}

bool Product::isInStock() { return stock > 0; }

bool Product::hasCategory(const string &category) {
    return find(categories.begin(), categories.end(), category) !=
           categories.end();
}
void Product::restockProduct(int amount) {
    if (amount > 0) {
        stock += amount;
    }
}

void Product::setName(const string &newName) { name = newName; }

void Product::setDescription(const string &newDescription) {
    description = newDescription;
}

void Product::addCategory(const string &category) {
    categories.push_back(category);
}

void Product::removeCategory(const string &category) {
    categories.erase(remove(categories.begin(), categories.end(), category),
                     categories.end());
}

const vector<string> &Product::getCategories() { return categories; }
