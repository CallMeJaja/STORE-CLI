#pragma once
#include "CategoryService.hpp"
#include "ProductService.hpp"
#include "TransactionService.hpp"
#include "UserService.hpp"
#include "map"

class AdminService {
  private:
    UserService &userService;
    TransactionService &transactionService;
    ProductService &productService;
    CategoryService &categoryService;

  public:
    AdminService(UserService &userService,
                 TransactionService &transactionService,
                 ProductService &productService,
                 CategoryService &categoryService);

    // Product Management
    bool addProduct(const string &name, int price, const string &description,
                    const vector<string> &categories, int stock);
    bool updateProduct(int productId, const string &name, int price,
                       const string &description, int stock);
    bool deleteProduct(int productId);
    bool restockProduct(int productId, int amount);
    int getTotalProducts();

    // Category Management
    bool addCategory(const string &name);
    bool updateCategory(int categoryId, const string &name);
    bool deleteCategory(int categoryId);
    vector<Category> getCategories();

    // Transaction
    vector<Transaction> getAllTransactions();
    double getTotalRevenue();

    // User Management
    bool updateUserStatus(int userId, bool isActive);
    bool toggleUserAccess(int userId);

    vector<User> getAllUsers();
    vector<Product> getAllProducts();
    vector<Product> getAllProductsByCategory(const string &category);
    vector<User> getActiveUsers();
    void clearCurrentUser();

    // Store Management
    // TODO Implement Update Store
};