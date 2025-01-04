#include "ShoppingMenu.hpp"
#include "../services/ShoppingService.hpp"
#include "../services/UserService.hpp"
#include "../utils/FormatHelper.hpp"
#include "../utils/InputValidator.hpp"
#include "string"

ShoppingMenu::ShoppingMenu(ShoppingService &shopService,
                           UserService &userService, MainMenu *mainMenu)
    : shopService(shopService), userService(userService), mainMenu(mainMenu) {}

void ShoppingMenu::display() {
    while (true) {
        clearScreen();
        cout << "Hello, " << userService.getCurrentUser()->fullName << "!"
             << endl;
        cout << "> Welcome to J-STORE <" << endl;
        cout << "\nTotal User Active: " << userService.getActiveUsers().size()
             << " users" << endl;
        cout << "Total Transactions Completed: "
             << userService.getTotalTransactions() << endl;
        cout << "\nYour Balance: "
             << FormatHelper::displayCurrency(
                    userService.getCurrentUser()->balance)
             << endl;
        cout << "\n1. Browse Categories" << endl;
        cout << "2. Transaction History" << endl;
        cout << "3. Top Up Balance" << endl;
        cout << "4. Edit Profile" << endl;
        cout << "5. Sign Out" << endl;

        int choice;
        if (!InputValidator::validateIntInput(choice, "\nEnter choice: "))
            continue;
        switch (choice) {
        case 1:
            browseCategories();
            break;
        case 2:
            viewTransactionHistory();
            break;
        case 3:
            topUpBalance();
            break;
        case 4:
            cout << "\nSigning out. Please wait..." << endl;
            userService.clearCurrentUser();
            mainMenu->displayMainMenu();
            break;
        default:
            cout << "[Error]: Invalid option. Please try again." << endl;
            break;
        }
    }
}

void ShoppingMenu::browseCategories() {
    clearScreen();
    cout << "> Categories List <\n" << endl;
    auto categories = shopService.getCategories();
    if (categories.empty()) {
        cout << "No categories available." << endl;
        pause();
        return;
    }

    for (int i = 0; i < categories.size(); i++) {
        cout << i << ". " << categories[i].name << endl;
    }
    cout << categories.size() << ". Back" << endl;

    int choice;
    if (!InputValidator::validateIntInput(choice, "\nEnter choice: "))
        return;
    if (choice == categories.size())
        return; // Back
    if (choice < 0 || choice >= categories.size()) {
        cout << "[Error]: Invalid option. Please try again." << endl;
        pause();
        return;
    }

    viewProducts(categories[choice].name);
}

void ShoppingMenu::viewProducts(const string &category) {
    clearScreen();
    cout << "> List Products in " << category << " <\n" << endl;

    auto products = shopService.getProductsByCategory(category);
    if (products.empty()) {
        cout << "No products available in this category." << endl;
        pause();
        return;
    }

    cout << "\nYour Balance: "
         << FormatHelper::displayCurrency(userService.getCurrentUser()->balance)
         << endl;
    cout << "\nProduct List:" << endl;
    for (int i = 0; i < products.size(); i++) {
        cout << i + 1 << ". " << products[i].name << endl;
        cout << "   Price: " << FormatHelper::displayCurrency(products[i].price)
             << endl;
        cout << "   Stock: " << products[i].stock << endl;
        cout << "   Description: " << products[i].description << endl;
    }
    cout << products.size() + 1 << ". Back" << endl;

    int choice;
    if (!InputValidator::validateIntInput(choice, "\nEnter choice: "))
        return;

    if (choice == products.size() + 1)
        return; // Back

    if (choice < 0 || choice >= products.size()) {
        cout << "\n[Error]: Invalid option. Please try again." << endl;
        pause();
        return;
    }

    int quantity;
    if (!InputValidator::validateIntInput(quantity, "\nEnter quantity: "))
        return;

    if (!shopService.isProductAvailabe(products[choice].id, quantity)) {
        cout << "\n[Error]: Product is not available or insufficient stock."
             << endl;
        pause();
        return;
    }

    int totalPrice = products[choice].price * quantity;
    cout << "\nTotal Price: " << FormatHelper::displayCurrency(totalPrice)
         << endl;
    cout << "Confirm purchase? (Y/N): ";
    char confirm;
    cin >> confirm;
    cin.ignore();

    if (tolower(confirm) == 'y') {
        if (shopService.purchaseProduct(userService.getCurrentUser()->id,
                                        products[choice - 1].id, quantity)) {
            cout << "\nPurchase successful!" << endl;
            pause();
        } else {
            cout << "\n[Error]: Insufficient balance." << endl;
            cout << "Would you like to top up your balance? (Y/N): ";
            char topUp;
            cin >> topUp;
            cin.ignore();
            if (tolower(topUp) == 'y') {
                topUpBalance();
            }
            pause();
        }
        sleep(1);
    }
}

void ShoppingMenu::topUpBalance() {
    while (true) {
        // clearScreen();
        cout << userService.getCurrentUser()->fullName << endl;
        cout << "> Top Up Balance <\n" << endl;
        cout << "Current Balance: "
             << FormatHelper::displayCurrency(
                    userService.getCurrentUser()->balance)
             << endl;
        cout << "\nSelect Amount:" << endl;
        cout << "1. Rp. 50.000" << endl;
        cout << "2. Rp. 100.000" << endl;
        cout << "3. Rp. 200.000" << endl;
        cout << "4. Custom Amount" << endl;
        cout << "5. Back" << endl;

        int choice;
        if (!InputValidator::validateIntInput(choice, "\nEnter choice: "))
            continue;

        int amount;
        switch (choice) {
        case 1:
            amount = 50000;
            break;
        case 2:
            amount = 100000;
            break;
        case 3:
            amount = 200000;
            break;
        case 4:
            if (!InputValidator::validateIntInput(
                    amount, "\nEnter custom amount (minimum Rp.10.000): "))
                continue;
            if (amount < 10000) {
                cout << "[Error]: Minimum top up amount is Rp.10.000." << endl;
                continue;
            }
            break;
        case 5:
            return;
        default:
            cout << "\n[Error]: Invalid option. Please try again." << endl;
            break;
        }

        if (userService.topUpBalance(userService.getCurrentUser()->id,
                                     amount)) {
            cout << "\nTop up successful!" << endl;
            cout << "New Balance: "
                 << FormatHelper::displayCurrency(
                        userService.getCurrentUser()->balance)
                 << endl;
            // pause();
            return;
        } else {
            cout << "\n[Error]: Top up failed. Please try again." << endl;
            // pause();
            return;
        }

        // if (InputValidator::validateConfirmation(
        //         "Confirm top up amount: " +
        //         FormatHelper::displayCurrency(amount) + " (Y/N): ")) {
        //     if
        //     (userService.topUpBalance(userService.getCurrentUser()->id,
        //                                  amount)) {
        //         cout << "\nTop up successful!" << endl;
        //         cout << "New Balance: "
        //              << FormatHelper::displayCurrency(
        //                     userService.getCurrentUser()->balance)
        //              << endl;
        //         pause();
        //         return;
        //     } else {
        //         cout << "\n[Error]: Top up failed. Please try again."
        //              << endl;
        //         pause();
        //         return;
        //     }
        // }
    }
}

void ShoppingMenu::viewTransactionHistory() {
    clearScreen();
    cout << "> Transaction History <\n" << endl;

    auto transactions =
        userService.getUserTransactions(userService.getCurrentUser()->id);
    if (transactions.empty()) {
        cout << "No transaction history available." << endl;
        pause();
        return;
    }

    cout << "Your Transaction History:" << endl;
    for (const auto &transaction : transactions) {
        cout << "\n" << string(50, '=') << endl;
        cout << "Transaction ID: " << transaction.id << endl;
        cout << "Product: " << transaction.productName << endl;
        cout << "Quantity: " << transaction.quantity << endl;
        cout << "Total Price: "
             << FormatHelper::displayCurrency(transaction.totalPrice) << endl;
        cout << "Transaction Date: " << transaction.createdAt << endl;
        cout << "\n" << string(50, '=') << endl;
    }
    pause();
}