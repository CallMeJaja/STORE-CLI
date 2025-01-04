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
                 CategoryService &categoryService)
        : userService(userService), transactionService(transactionService),
          productService(productService), categoryService(categoryService) {}

    // Dashboard Statistics
    map<string, int> getDashboardStats() {
        return {
            {"totalUsers", userService.getTotalUsers()},
            {"activeUsers", userService.getActiveUsers().size()},
            {"totalProducts", productService.getTotalProducts()},
            {"totalCategories", categoryService.getTotalCategories()},
            {"totalTransactions", transactionService.getTotalTransactions()},
        };
    }

    // Revenue Analytics
    double getTotalRevenue() { return transactionService.getTotalRevenue(); }

    vector<Product> getLowStockProducts() {
        return productService.getLowStockProducts();
    }

    bool toggleUserAccess(int userId) {
        return userService.toggleUserStatus(userId);
    }

    vector<User> getAllUsers() { return userService.getUsers(); }

    vector<Transaction> getAllTransactions() {
        return transactionService.getAllTransactions();
    }

    map<string, double> getRevenueStats() {
        auto transactions = transactionService.getAllTransactions();
        double totalRevenue = 0.0;
        double avgTransactionValue = 0.0;

        if (!transactions.empty()) {
            totalRevenue = getTotalRevenue();
            avgTransactionValue = totalRevenue / transactions.size();
        }

        return {{"totalRevenue", totalRevenue},
                {"avgTransactionValue", avgTransactionValue}};
    }

    vector<pair<string, int>> getTopSellingProducts(int limit = 5) {
        auto products = productService.getProducts();
        vector<pair<string, int>> productSales;

        for (const auto &product : products) {
            productSales.push_back({product.name, product.sold});
        }

        sort(productSales.begin(), productSales.end(),
             [](const auto &a, const auto &b) { return a.second > b.second; });

        if (productSales.size() > limit) {
            productSales.resize(limit);
        }

        return productSales;
    }

    map<string, int> getCategoryStats() {
        map<string, int> stats;
        auto categories = categoryService.getActiveCategories();

        for (const auto &category : categories) {
            auto products = productService.getProductsByCategory(category.name);
            stats[category.name] = products.size();
        }
        return stats;
    }

    bool validateUserActions(int userId, int daysBack = 7) {
        auto user = userService.findById(userId);
        if (!user)
            return false;

        auto transactions = transactionService.getUserTransactions(userId);
        time_t now = time(nullptr);
        int recentTransactions = count_if(
            transactions.begin(), transactions.end(),
            [now, daysBack](const Transaction &t) {
                return difftime(now, t.createdAt) <= daysBack * 24 * 36000;
            });

        return recentTransactions > 0;
    }

    vector<User> getInactiveUsers(int dayThreshold = 7) {
        auto users = userService.getUsers();
        vector<User> inactiveUsers;

        copy_if(users.begin(), users.end(), back_inserter(inactiveUsers),
                [this, dayThreshold](const User &user) {
                    return !validateUserActions(user.id, dayThreshold);
                });
        return inactiveUsers;
    }

    vector<pair<int, double>> getUserSpendingReport() {
        auto users = userService.getUsers();
        vector<pair<int, double>> report;

        for (const auto &user : users) {
            double totalSpent = userService.getUserTotalSpent(user.id);
            report.push_back({user.id, totalSpent});
        }

        sort(report.begin(), report.end(),
             [](const auto &a, const auto &b) { return a.second > b.second; });

        return report;
    }

    bool archiveInactiveUsers(int dayInactive = 7) {
        auto inactiveUsers = getInactiveUsers(dayInactive);
        bool success = true;

        for (const auto &user : inactiveUsers) {
            if (!userService.toggleUserStatus(user.id)) {
                success = false;
            }
        }

        return success;
    }

    map<string, int> getTransactionsStats() {
        auto transactions = transactionService.getAllTransactions();
        int totalTransactions = transactions.size();
        int completedTransactions =
            count_if(transactions.begin(), transactions.end(),
                     [](const Transaction &t) { return t.isCompleted; });

        return {{"total", totalTransactions},
                {"completed", completedTransactions},
                {"pending", totalTransactions - completedTransactions}};
    }

    vector<pair<string, double>> getProductPerformance() {
        auto products = productService.getProducts();
        vector<pair<string, double>> performance;

        for (const auto &product : products) {
            double soldPercentege =
                product.getTotalQuantity() > 0
                    ? (double)product.sold / product.getTotalQuantity() * 100
                    : 0.0;
            performance.push_back({product.name, soldPercentege});
        }

        sort(performance.begin(), performance.end(),
             [](const auto &a, const auto &b) { return a.second > b.second; });

        return performance;
    }

    // Product Management
    bool addProduct(const string &name, int price, const string &description,
                    const vector<string> &categories, int stock) {
        return productService.addProduct(name, price, description, categories,
                                         stock);
    }

    bool updateProduct(int productId, const string &name, int price,
                       const string &description, int stock) {
        return productService.updateProduct(productId, name, price, description,
                                            stock);
    }

    bool deleteProduct(int productId) {
        return productService.deleteProduct(productId);
    }

    bool restockProduct(int productId, int amount) {
        return productService.restockProduct(productId, amount);
    }

    // Category Management
    bool addCategory(const string &name) {
        return categoryService.addCategory(name);
    }

    bool updateCategory(int categoryId, const string &name) {
        return categoryService.updateCategory(categoryId, name);
    }

    bool deleteCategory(int categoryId) {
        return categoryService.deleteCategory(categoryId);
    }

    vector<Category> getCategories() {
        return categoryService.getActiveCategories();
    }

    // User Management
    bool updateUserStatus(int userId, bool isActive) {
        auto user = userService.findById(userId);
        if (!user)
            return false;

        user->isActive = isActive;
        return userService.updateUser(*user);
    }

    User *findUserById(int userId) { return userService.findById(userId); }

    vector<User> getAllUsers() { return userService.getUsers(); }

    vector<Product> getAllProducts() { return productService.getProducts(); }

    vector<User> getActiveUsers() { return userService.getActiveUsers(); }
};