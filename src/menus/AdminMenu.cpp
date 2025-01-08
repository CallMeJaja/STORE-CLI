#include "menus/AdminMenu.hpp"
#include "algorithm"
#include "conio.h"
#include "iomanip"
#include "services/AdminService.hpp"
#include "unistd.h"
#include "utils/FormatHelper.hpp"
#include "utils/InputValidator.hpp"

AdminMenu::AdminMenu(AdminService &adminService, MainMenu *mainMenu)
    : adminService(adminService), mainMenu(mainMenu) {}

void AdminMenu::manageProducts() {
    clearScreen();
    // InputValidator::clearInputBuffer();

    cout << "> Manage Products <" << endl;
    cout << "\n1. Add Product" << endl;
    cout << "2. Update Product" << endl;
    cout << "3. Delete Product" << endl;
    cout << "4. List Products" << endl;
    cout << "5. Back" << endl;

    int choice;

    while (!InputValidator::validateIntInput(choice, "\nEnter choice: ", 5)) {
        return;
    }

    switch (choice) {
    case 1:
        addProduct();
        return;
    case 2:
        updateProduct();
        return;
    case 3:
        deleteProduct();
        return;
    case 4:
        listProducts();
        return;
    case 5:
        return;
    }
}

void AdminMenu::manageCategories() {
    while (true) {
        clearScreen();
        cout << "> Manage Categories <" << endl;
        cout << "\n1. Add Category" << endl;
        cout << "2. Update Category" << endl;
        cout << "3. Delete Category" << endl;
        cout << "4. Back" << endl;

        int choice;

        while (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
            break;
        }

        switch (choice) {
        case 1:
            addCategory();
            return;
        case 2:
            updateCategory();
            return;
        case 3:
            deleteCategory();
            return;
        case 4:
            return;
        }
    }
}

void AdminMenu::manageUsers() {
    while (true) {
        clearScreen();
        cout << "> Manage Users <" << endl;
        cout << "\n1. View Users" << endl;
        cout << "2. Toggle User Access" << endl;
        cout << "3. Back" << endl;

        int choice;

        while (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
            break;
        }

        switch (choice) {
        case 1:
            viewUsers();
            return;
        case 2:
            toggleUserAccess();
            return;
        case 3:
            return;
        }
    }
}

void AdminMenu::listProducts() {
    clearScreen();
    cout << "> List Products <" << endl;
    auto products = adminService.getAllProducts();
    if (products.empty()) {
        cout << "No products available." << endl;
        pause();
        return;
    }

    int pageSize = 10;
    int currentPage = 0;
    int totalPages = (products.size() + pageSize - 1) / pageSize;

    while (true) {
        clearScreen();
        int startIndex = currentPage * pageSize;
        int endIndex =
            min(startIndex + pageSize, static_cast<int>(products.size()));

        cout << "> List Products - Page " << currentPage + 1 << " of "
             << totalPages << endl;
        cout << left << setw(5) << "ID" << setw(25) << "Name" << setw(15)
             << "Price" << setw(10) << "Stock" << endl;
        cout << string(50, '-') << endl;

        for (int i = startIndex; i < endIndex; i++) {
            cout << left << setw(5) << i + 1 << setw(25) << products[i].name
                 << setw(15) << FormatHelper::displayCurrency(products[i].price)
                 << setw(10) << products[i].stock << endl;
        }

        cout << string(50, '-') << endl;
        cout << "\nPress ";

        if (currentPage > 0)
            cout << "[p] Previous, ";
        if (currentPage < totalPages - 1)
            cout << "[n] Next, ";
        cout << "[b] to Back: ";

        string input = "";
        while (input.empty()) {
            if (_kbhit()) {
                input += _getch();
            }
            sleep(0.1);
            cout << input;
        }

        if (input == "b") {
            return;
        }

        if (currentPage < totalPages - 1 && input == "n") {
            currentPage++;
            continue;
        } else if (currentPage > 0 && input == "p") {
            currentPage--;
            continue;
        } else {
            cout << "\n[Error]: Invalid input. Please choose ";
            if (currentPage > 0)
                cout << "[p], ";
            if (currentPage < totalPages - 1)
                cout << "[n] ";
            cout << "or [b]\n\n";
            pause();
            continue;
        }
    }
    pause();
}

void AdminMenu::addProduct() {
    clearScreen();
    InputValidator::clearInputBuffer();

    cout << "> Add Product <" << endl;
    string name, description;
    int price, stock;
    vector<string> categories = {"All Products"};

    while (
        !InputValidator::validateStringInput(name, "\nEnter product name: ") ||
        !InputValidator::validateIntInput(price, "\nEnter product price: ") ||
        !InputValidator::validateStringInput(description,
                                             "\nEnter product description: ") ||
        !InputValidator::validateIntInput(stock, "\nEnter product stock: ")) {
        return;
    }

    // category selection
    auto availableCategories = adminService.getCategories();
    cout << "\nAvailable Categories:" << endl;
    for (int i = 0; i < availableCategories.size(); i++) {
        if (availableCategories[i].name != "All Products") {
            cout << i << ". " << availableCategories[i].name << endl;
        }
    }

    int choice;

    while (!InputValidator::validateIntInput(
        choice, "\nSelect additional category (0 to skip): ",
        availableCategories.size() - 1)) {

        if (choice == 0) {
            break;
        }
    }

    if (choice != 0) {
        categories.push_back(availableCategories[choice].name);
    }

    if (adminService.addProduct(name, price, description, categories, stock)) {
        cout << "\nProduct added successfully!" << endl;
        sleep(1);
        return;
    } else {
        cout << "\n[Error]: Failed to add product. Please try again." << endl;
        sleep(1);
        return;
    }
}

void AdminMenu::updateProduct() {
    clearScreen();
    int categoryId, productId;
    string category;
    cout << "> Update Product <\n" << endl;
    auto categories = adminService.getCategories();

    displayAllCategories();

    while (!InputValidator::validateIntInput(
        categoryId, "\nEnter Category ID (Back = 0)", categories.size())) {
        return;
    }

    if (categoryId == 0) {
        return;
    }

    auto categoryIt = find_if(
        categories.begin(), categories.end(),
        [categoryId](const Category &cat) { return cat.id == categoryId; });

    category = categoryIt->name;

    auto products = adminService.getAllProductsByCategory(category);

    if (products.empty()) {
        cout << "No products available." << endl;
        pause();
        return;
    }

    for (int i = 0; i < products.size(); i++) {
        cout << i + 1 << ". " << products[i].name << endl;
        cout << "   Price: " << FormatHelper::displayCurrency(products[i].price)
             << endl;
        cout << "   Stock: " << products[i].stock << endl;
    }

    while (!InputValidator::validateIntInput(
        productId, "\nEnter Product ID (Back = 0): ", products.size())) {
        return;
    }

    if (productId == 0) {
        return;
    }

    auto &product = products[productId - 1];

    InputValidator::clearInputBuffer();
    cout << "\nUpdate Product: " << product.name << endl;

    string name, description, priceStr, stockStr;

    while (!InputValidator::validateStringInput(
               name, "\nEnter New Name (current: " + product.name + "): ") ||
           !InputValidator::validateStringInput(
               description, "\nEnter New Description (current: " +
                                product.description + "): ") ||
           !InputValidator::validateStringInput(
               priceStr, "\nEnter New Price (current: " +
                             FormatHelper::displayCurrency(product.price) +
                             "): ") ||
           !InputValidator::validateStringInput(
               stockStr, "\nEnter New Stock (current: " +
                             to_string(product.stock) + "): ")) {
        return;
    }

    product.name = name;
    product.description = description;
    product.price = stoi(priceStr);
    product.stock = stoi(stockStr);

    if (adminService.updateProduct(product.id, product.name, product.price,
                                   product.description, product.stock)) {
        FormatHelper::displayMessage("info", "Product updated successfully!");
        sleep(1);
        return;
    } else {
        FormatHelper::displayMessage(
            "error", "Failed to update product. Please try again.");
        sleep(1);
        return;
    }
}

void AdminMenu::deleteProduct() {
    clearScreen();
    InputValidator::clearInputBuffer();
    cout << "> Delete Product <\n" << endl;

    auto products = adminService.getAllProducts();

    if (products.empty()) {
        cout << "No products available." << endl;
        pause();
        return;
    }

    for (int i = 0; i < products.size(); i++) {
        cout << i + 1 << ". " << products[i].name << endl;
        cout << "   Price: " << FormatHelper::displayCurrency(products[i].price)
             << endl;
        cout << "   Stock: " << products[i].stock << endl;
    }

    int productId;
    while (!InputValidator::validateIntInput(
        productId, "\nEnter Product ID: ", products.size())) {
        return;
    }

    auto &product = products[productId - 1];

    if (InputValidator::validateConfirmation(
            "Are you sure you want to delete " + product.name + "? (Y/N): ")) {
        if (adminService.deleteProduct(product.id)) {
            cout << "\n[Info]: Product deleted successfully!" << endl;
        } else {
            cout << "\n[Error]: Failed to delete product. Please try again."
                 << endl;
        }
    } else {
        cout << "\n[Info]: Product deletion canceled." << endl;
    }

    pause();
}

void AdminMenu::listCategories() {
    clearScreen();
    cout << "> List Categories \n<" << endl;
    auto categories = adminService.getCategories();

    if (categories.empty()) {
        cout << "No categories available." << endl;
        pause();
        return;
    }

    for (int i = 0; i < categories.size(); i++) {
        cout << i + 1 << ". " << categories[i].name << endl;
    }

    pause();
}

void AdminMenu::addCategory() {
    clearScreen();
    cout << "> Add Category <" << endl;
    string name;

    // TODO Refactor Input
    while (true) {
        if (!InputValidator::validateStringInput(name,
                                                 "\nEnter category name: ")) {
            return;
        }

        if (adminService.addCategory(name)) {
            cout << "\n[Info]: Category added successfully!" << endl;
            pause();
            return;
        } else {
            cout << "[Error]: Category already exists. Please try again."
                 << endl;
            continue;
        }
    }
}

void AdminMenu::updateCategory() {
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
    cout << "\n> Update Category: " << category.name << endl;

    string name;
    if (InputValidator::validateStringInput(
            name, "\nNew Name (current: " + category.name + "): ")) {
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

void AdminMenu::deleteCategory() {
    clearScreen();
    cout << "> Delete Category <\n" << endl;

    auto categories = adminService.getCategories();
    if (categories.empty()) {
        cout << "No categories available." << endl;
        pause();
        return;
    }

    for (int i = 0; i < categories.size(); i++) {
        if (categories[i].name != "All Products") {
            cout << i << ". " << categories[i].name << endl;
        }
    }

    int choice;
    while (true) {
        if (!InputValidator::validateIntInput(choice,
                                              "\nEnter choice (Back = 0): ")) {
            return;
        }

        if (choice == 0) {
            return;
        }

        if (choice < 1 || choice > categories.size() - 1) {
            cout << "[Error]: Invalid option. Please try again." << endl;
            continue;
        } else {
            break;
        }
    }

    if (InputValidator::validateConfirmation(
            "Are you sure you want to delete " + categories[choice].name +
            "? (Y/N): ")) {
        if (adminService.deleteCategory(categories[choice].id)) {
            cout << "\nCategory deleted successfully!" << endl;
            pause();
            return;
        } else {
            cout << "[Error]: Failed to delete category. Please try again."
                 << endl;
        }
    }
}

void AdminMenu::viewUsers() {
    clearScreen();
    cout << "> View Users <" << endl;

    auto users = adminService.getAllUsers();
    if (users.empty()) {
        cout << "No users available." << endl;
        pause();
        return;
    }

    for (int i = 0; i < users.size(); i++) {
        if (!users[i].isAdmin) {
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
    }
    pause();
}

void AdminMenu::toggleUserAccess() {
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

void AdminMenu::viewReport() {
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
             << FormatHelper::displayCurrency(transaction.totalPrice) << endl;
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

void AdminMenu::display() {
    while (true) {
        clearScreen();
        cout << "> Admin Panel - J-STORE <" << endl;
        cout << "\n1. Manage Products" << endl;
        cout << "2. Manage Categories" << endl;
        cout << "3. Manage Users" << endl;
        cout << "4. View Reports" << endl;
        cout << "5. Sign Out" << endl;

        int choice;

        while (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
            if (choice >= 1 && choice <= 5) {
                break;
            } else {
                cout << "[Error]: Invalid option. Please try again." << endl;
                continue;
            }
        }

        switch (choice) {
        case 1:
            manageProducts();
            break; // FIXME Return menu bug
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
            adminService.clearCurrentUser();
            sleep(1);
            mainMenu->displayMainMenu();
            return;
        }
    }
}

void AdminMenu::displayAllProducts() {
    auto products = adminService.getAllProducts();
    if (products.empty()) {
        cout << "No products available." << endl;
        pause();
        return;
    }

    for (int i = 0; i < products.size(); i++) {
        cout << i + 1 << ". " << products[i].name << "(" << endl;
        cout << i + 1 << ". " << products[i].name << endl;
        cout << "   Price: " << FormatHelper::displayCurrency(products[i].price)
             << endl;
        cout << "   Stock: " << products[i].stock << endl;
    }
}

void AdminMenu::displayAllCategories() {
    auto categories = adminService.getCategories();
    if (categories.empty()) {
        cout << "No categories available." << endl;
        pause();
        return;
    }

    for (size_t i = 0; i < categories.size(); ++i) {
        cout << i + 1 << ". " << categories[i].name << endl;
    }
}