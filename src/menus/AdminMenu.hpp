#pragma once
#include "../services/AdminService.hpp"
#include "../utils/FormatHelper.hpp"
#include "../utils/InputValidator.hpp"
#include "MenuBase.hpp"
#include <unistd.h>

class AdminMenu : public MenuBase {
  private:
    AdminService &adminService;

    // Management Methods
    void manageProducts() {
        while (true) {
            clearScreen();
            cout << "> Manage Products <" << endl;
            cout << "\n1. Add Product" << endl;
            cout << "2. Update Product" << endl;
            cout << "3. Delete Product" << endl;
            cout << "4. Back" << endl;

            int choice;
            if (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
                continue;
            }

            switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                updateProduct();
                break;
            case 3:
                deleteProduct();
                break;
            case 4:
                return;
            default:
                cout << "\n[Error]: Invalid option. Please try again." << endl;
                break;
            }
        }
    }

    void manageCategories() {
        while (true) {
            clearScreen();
            cout << "> Manage Categories <" << endl;
            cout << "\n1. Add Category" << endl;
            cout << "2. Update Category" << endl;
            cout << "3. Delete Category" << endl;
            cout << "4. Back" << endl;

            int choice;
            if (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
                continue;
            }

            switch (choice) {
            case 1:
                addCategory();
                break;
            case 2:
                updateCategory();
                break;
            case 3:
                deleteCategory();
                break;
            case 4:
                return;
            default:
                cout << "\n[Error]: Invalid option. Please try again." << endl;
                break;
            }
        }
    }

    void manageUsers() {
        while (true) {
            clearScreen();
            cout << "> Manage Users <" << endl;
            cout << "\n1. View Users" << endl;
            cout << "2. Toggle User Access" << endl;
            cout << "3. Back" << endl;

            int choice;
            if (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
                continue;
            }

            switch (choice) {
            case 1:
                viewUsers();
                break;
            case 2:
                toggleUserAccess();
                break;
            case 3:
                return;
            default:
                cout << "\n[Error]: Invalid option. Please try again." << endl;
                break;
            }
        }
    }

    // Product Operation
    void addProduct() {
        clearScreen();
        cout << "> Add Product <\n" << endl;
        string name, description;
        int price, stock;
        vector<string> categories = {"All Products"};

        if (!InputValidator::validateStringInput(name,
                                                 "Enter product name: ")) {
            return;
        }

        if (!InputValidator::validateIntInput(price, "Enter product price: ")) {
            return;
        }

        if (!InputValidator::validateStringInput(
                description, "Enter product description: ")) {
            return;
        }

        if (!InputValidator::validateIntInput(stock, "Enter product stock: ")) {
            return;
        }

        // category selection
        auto availableCategories = adminService.getCategories();
        if (!availableCategories.empty()) {
            cout << "\nAvailable Categories:" << endl;
            for (int i = 0; i < availableCategories.size(); i++) {
                cout << i + 1 << ". " << availableCategories[i].name << endl;
            }

            int choice;
            if (!InputValidator::validateIntInput(
                    choice, "Select additional category (0 to skip): ")) {
                return;
            }

            if (choice > 0 && choice <= availableCategories.size()) {
                categories.push_back(availableCategories[choice - 1].name);
            }
        }

        if (adminService.addProduct(name, price, description, categories,
                                    stock)) {
            cout << "\nProduct added successfully!" << endl;
        } else {
            cout << "\n[Error]: Failed to add product. Please try again."
                 << endl;
        }
        pause();
    }

    void updateProduct() {
        clearScreen();
        cout << "> Update Product <\n" << endl;

        auto products = adminService.getAllProducts();
        if (products.empty()) {
            cout << "No products available." << endl;
            pause();
            return;
        }

        for (int i = 0; i < products.size(); i++) {
            cout << i + 1 << ". " << products[i].name << endl;
            cout << "   Price: "
                 << FormatHelper::displayCurrency(products[i].price) << endl;
            cout << "   Stock: " << products[i].stock << endl;
        }

        int choice;
        if (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
            return;
        }
        if (choice < 1 || choice > products.size()) {
            cout << "\n[Error]: Invalid option. Please try again." << endl;
            pause();
            return;
        }

        auto &product = products[choice - 1];
        cout << "\nUpdate Product: " << product.name << endl;
        cout << "Leave blank to keep current value.\n" << endl;

        string name, description, priceStr, stockStr;

        cout << "Current Name: " << product.name << endl;
        if (InputValidator::validateStringInput(name, "New Name: ")) {
            product.name = name;
        }

        cout << "Current Description: " << product.description << endl;
        if (InputValidator::validateStringInput(description,
                                                "New Description: ")) {
            product.description = description;
        }

        cout << "Current Price: "
             << FormatHelper::displayCurrency(product.price) << endl;
        if (InputValidator::validateStringInput(priceStr, "New Price: ")) {
            product.price = stoi(priceStr);
        }

        cout << "Current Stock: " << product.stock << endl;
        if (InputValidator::validateStringInput(stockStr, "New Stock: ")) {
            product.stock = stoi(stockStr);
        }

        if (adminService.updateProduct(product.id, product.name, product.price,
                                       product.description, product.stock)) {
            cout << "\nProduct updated successfully!" << endl;
        } else {
            cout << "\n[Error]: Failed to update product. Please try again."
                 << endl;
        }
        pause();
    }

    void deleteProduct() {
        clearScreen();
        cout << "> Delete Product <\n" << endl;

        auto products = adminService.getAllProducts();
        if (products.empty()) {
            cout << "No products available." << endl;
            pause();
            return;
        }

        for (int i = 0; i < products.size(); i++) {
            cout << i + 1 << ". " << products[i].name << endl;
            cout << "   Price: "
                 << FormatHelper::displayCurrency(products[i].price) << endl;
            cout << "   Stock: " << products[i].stock << endl;
        }

        int choice;
        if (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
            return;
        }

        if (choice < 1 || choice > products.size()) {
            cout << "\n[Error]: Invalid option. Please try again." << endl;
            pause();
            return;
        }

        if (InputValidator::validateConfirmation(
                "Are you sure you want to delete " + products[choice - 1].name +
                "? (Y/N): ")) {
            if (adminService.deleteProduct(products[choice - 1].id)) {
                cout << "\nProduct deleted successfully!" << endl;
            } else {
                cout << "\n[Error]: Failed to delete product. Please try again."
                     << endl;
            }
        }
        pause();
    }

    // Category Operation
    void addCategory() {
        clearScreen();
        cout << "> Add Category <\n" << endl;
        string name;

        if (!InputValidator::validateStringInput(name,
                                                 "Enter category name: ")) {
            return;
        }

        if (adminService.addCategory(name)) {
            cout << "\nCategory added successfully!" << endl;
        } else {
            cout << "[Error] Category already exists. Please try again.\n"
                 << endl;
        }
    }

    void updateCategory() {
        clearScreen();
        cout << "> Update Category <\n" << endl;

        auto categories = adminService.getCategories();
        if (categories.empty()) {
            cout << "No categories available." << endl;
            pause();
            return;
        }

        for (int i = 0; i < categories.size(); i++) {
            cout << i + 1 << ". " << categories[i].name << endl;
        }

        int choice;
        if (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
            return;
        }

        if (choice < 1 || choice > categories.size()) {
            cout << "\n[Error]: Invalid option. Please try again." << endl;
            pause();
            return;
        }

        auto &category = categories[choice - 1];
        cout << "\nUpdate Category: " << category.name << endl;
        cout << "Leave blank to keep current value.\n" << endl;

        string name;
        cout << "Current Name: " << category.name << endl;
        if (InputValidator::validateStringInput(name, "New Name: ")) {
            category.name = name;
        }

        if (adminService.updateCategory(category.id, category.name)) {
            cout << "\nCategory updated successfully!" << endl;
        } else {
            cout << "\n[Error]: Failed to update category. Please try again."
                 << endl;
        }
        pause();
    }

    void deleteCategory() {
        clearScreen();
        cout << "> Delete Category <\n" << endl;

        auto categories = adminService.getCategories();
        if (categories.empty()) {
            cout << "No categories available." << endl;
            pause();
            return;
        }

        for (int i = 0; i < categories.size(); i++) {
            cout << i + 1 << ". " << categories[i].name << endl;
        }

        int choice;
        if (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
            return;
        }

        if (choice < 1 || choice > categories.size()) {
            cout << "\n[Error]: Invalid option. Please try again." << endl;
            pause();
            return;
        }

        if (InputValidator::validateConfirmation(
                "Are you sure you want to delete " +
                categories[choice - 1].name + "? (Y/N): ")) {
            if (adminService.deleteCategory(categories[choice - 1].id)) {
                cout << "\nCategory deleted successfully!" << endl;
            } else {
                cout
                    << "\n[Error]: Failed to delete category. Please try again."
                    << endl;
            }
        }

        // cout << "\nAre you sure you want to delete "
        //      << categories[choice - 1].name << "? (Y/N): ";
        // char confirm;
        // cin >> confirm;
        // cin.ignore();

        // if (tolower(confirm) == 'y') {
        //     if (adminService.deleteCategory(categories[choice - 1].id)) {
        //         cout << "\nCategory deleted successfully!" << endl;
        //     } else {
        //         cout
        //             << "\n[Error]: Failed to delete category. Please try
        //             again."
        //             << endl;
        //     }
        // }
    }

    // User Operation
    void viewUsers() {
        clearScreen();
        cout << "> View Users <" << endl;

        auto users = adminService.getAllUsers();
        if (users.empty()) {
            cout << "No users available." << endl;
            pause();
            return;
        }

        for (int i = 0; i < users.size(); i++) {
            cout << i + 1 << ". " << users[i].fullName << endl;
            cout << "   Email: " << users[i].email << endl;
            cout << "   Balance: "
                 << FormatHelper::displayCurrency(users[i].balance) << endl;
            cout << "   Total Transactions: " << users[i].totalTransactions
                 << endl;
            cout << "   Status: " << (users[i].isActive ? "Active" : "Inactive")
                 << endl;
            cout << "   Role: " << (users[i].isAdmin ? "Admin" : "User")
                 << endl;
        }
        pause();
    }

    void toggleUserAccess() {
        clearScreen();
        cout << "> Toggle User Access <" << endl;

        auto users = adminService.getAllUsers();
        if (users.empty()) {
            cout << "No users available." << endl;
            pause();
            return;
        }

        for (int i = 0; i < users.size(); i++) {
            cout << i + 1 << ". " << users[i].fullName << endl;
            cout << "   Email: " << users[i].email << endl;
            cout << "   Status: " << (users[i].isActive ? "Active" : "Inactive")
                 << endl;
        }

        int choice;
        if (!InputValidator::validateIntInput(choice, "\nSelect user: ")) {
            return;
        }

        if (choice < 1 || choice > users.size()) {
            cout << "\n[Error]: Invalid option. Please try again." << endl;
            pause();
            return;
        }

        auto &selectedUser = users[choice - 1];
        string newStatus = selectedUser.isActive ? "deactivate" : "activate";
        if (InputValidator::validateConfirmation(
                "Are you sure you want to " + newStatus + " " +
                selectedUser.fullName + "'s account? (Y/N): ")) {
            if (adminService.toggleUserAccess(selectedUser.id)) {
                cout << "\nUser status updated successfully!" << endl;
            } else {
                cout << "\n[Error]: Failed to update user status. Please try "
                        "again."
                     << endl;
            }
        }
        pause();
    }

    void viewReport() {
        clearScreen();
        cout << "> Transaction Report <\n" << endl;

        auto transactions = adminService.getAllTransactions();
        if (transactions.empty()) {
            cout << "No transactions available." << endl;
            pause();
            return;
        }

        int totalTransactions = transactions.size();
        double totalRevenue = adminService.getTotalRevenue();
        double avgTransactionValue = totalRevenue / totalTransactions;

        cout << "Transaction History:\n" << endl;
        for (const auto &transaction : transactions) {
            cout << "Transaction ID: " << transaction.id << endl;
            cout << "User ID: " << transaction.userId << endl;
            cout << "Product ID: " << transaction.productId << endl;
            cout << "Quantity: " << transaction.quantity << endl;
            cout << "Total Price: "
                 << FormatHelper::displayCurrency(transaction.totalPrice)
                 << endl;
            cout << "Date: " << FormatHelper::formatDate(transaction.createdAt)
                 << endl;
            cout << string(50, '=') << endl;
        }

        cout << "Total Users: " << adminService.getAllUsers().size() << " users"
             << endl;
        cout << "Total Transactions: " << totalTransactions << endl;
        cout << "Total Revenue: " << FormatHelper::displayCurrency(totalRevenue)
             << endl;
        cout << "Average Transaction Value: "
             << FormatHelper::displayCurrency(avgTransactionValue) << endl;

        pause();
    }

  public:
    AdminMenu(AdminService &adminService) : adminService(adminService) {}

    void display() {
        while (true) {
            clearScreen();
            cout << "> Admin Menu <" << endl;
            cout << "\n1. Manage Products" << endl;
            cout << "2. Manage Categories" << endl;
            cout << "3. Manage Users" << endl;
            cout << "4. View Reports" << endl;
            cout << "5. Sign Out" << endl;

            int choice;
            if (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
                continue;
            }

            switch (choice) {
            case 1:
                manageProducts();
                break;
            case 2:
                manageCategories();
                break;
            case 3:
                manageUsers();
                break;
            case 4:
                viewReport();
                break;
            case 5:
                cout << "\nSigning out from admin panel..." << endl;
                sleep(1);
                return;
            default:
                cout << "[Error]: Invalid option. Please try again." << endl;
                sleep(1);
            }
        }
    }
};