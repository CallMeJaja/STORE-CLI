#include "services/AdminService.hpp"

AdminService::AdminService(UserService &userService,
                           TransactionService &transactionService,
                           ProductService &productService,
                           CategoryService &categoryService)
    : userService(userService), transactionService(transactionService),
      productService(productService), categoryService(categoryService) {}

// Product Management
bool AdminService::addProduct(const string &name, int price,
                              const string &description,
                              const vector<string> &categories, int stock) {
    return productService.addProduct(name, price, description, categories,
                                     stock);
}

bool AdminService::updateProduct(int productId, const string &name, int price,
                                 const string &description, int stock) {
    return productService.updateProduct(productId, name, price, description,
                                        stock);
}

bool AdminService::deleteProduct(int productId) {
    return productService.deleteProduct(productId);
}

bool AdminService::restockProduct(int productId, int amount) {
    return productService.restockProduct(productId, amount);
}

int AdminService::getTotalProducts() {
    return productService.getTotalProducts();
}

// Category Management
bool AdminService::addCategory(const string &name) {
    return categoryService.addCategory(name);
}

bool AdminService::updateCategory(int categoryId, const string &name) {
    return categoryService.updateCategory(categoryId, name);
}

bool AdminService::deleteCategory(int categoryId) {
    return categoryService.deleteCategory(categoryId);
}

vector<Category> AdminService::getCategories() {
    return categoryService.getActiveCategories();
}

// Transaction
vector<Transaction> AdminService::getAllTransactions() {
    return transactionService.getAllTransactions();
}

double AdminService::getTotalRevenue() {
    return transactionService.getTotalRevenue();
}

// User Management
bool AdminService::updateUserStatus(int userId, bool isActive) {
    return userService.toggleUserStatus(userId);
}

bool AdminService::toggleUserAccess(int userId) {
    return userService.toggleUserStatus(userId);
}

vector<User> AdminService::getAllUsers() { return userService.getUsers(); }

vector<Product> AdminService::getAllProducts() {
    return productService.getProducts();
}

vector<Product> AdminService::getAllProductsByCategory(const string &category) {
    return productService.getProductsByCategory(category);
}

vector<User> AdminService::getActiveUsers() {
    return userService.getActiveUsers();
}

void AdminService::clearCurrentUser() { return userService.clearCurrentUser(); }