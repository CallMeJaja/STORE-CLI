#include "menus/ShoppingMenu.hpp"
#include "services/ShoppingService.hpp"
#include "services/UserService.hpp"
#include "string"
#include "utils/FormatHelper.hpp"
#include "utils/InputValidator.hpp"

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
        while (true) {
            if (!InputValidator::validateIntInput(choice, "\nEnter choice: "))
                continue;

            switch (choice) {
            case 1:
                browseCategories();
                return;
            case 2:
                viewTransactionHistory();
                return;
            case 3:
                topUpBalance();
                return;
            case 4:
                updateProfile();
                return;
            case 5:
                cout << "\nSigning out. Please wait..." << endl;
                userService.clearCurrentUser();
                mainMenu->displayMainMenu();
                return;
            default:
                cout << "[Error]: Invalid option. Please try again." << endl;
                continue;
            }
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
        cout << i + 1 << ". " << categories[i].name << endl;
    }
    cout << categories.size() + 1 << ". Back to Menu" << endl;

    int choice;
    while (true) {
        if (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
            return;
        }

        if (choice == categories.size() + 1) {
            return;
        }

        if (choice < 0 || choice >= categories.size()) {
            cout << "[Error]: Invalid option. Please try again." << endl;
            continue;
        } else {
            break;
        }
    }

    viewProducts(categories[choice - 1].name);
}

void ShoppingMenu::viewProducts(const string &category) {
    clearScreen();
    cout << "> List Products in " << category << " <" << endl;

    auto products = shopService.getProductsByCategory(category);
    if (products.empty()) {
        cout << "\nNo products available in this category." << endl;
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
    cout << products.size() + 1 << ". Back to Menu" << endl;

    int choice;
    while (true) {
        if (!InputValidator::validateIntInput(choice, "\nEnter Product ID: ")) {
            break;
        }

        if (choice == products.size() + 1)
            return;

        if (choice < 0 || choice >= products.size() + 1) {
            cout << "[Error]: Invalid option. Please try again." << endl;
            continue;
        } else {
            break;
        }
    }

    int quantity;
    while (true) {
        if (!InputValidator::validateIntInput(
                quantity,
                "\nEnter quantity for " + products[choice - 1].name + ": ")) {
            break;
        }

        if (products[choice - 1].stock >= quantity) {
            break;
        } else {
            cout << "[Error]: Product is not available or insufficient stock."
                 << endl;
            continue;
        }
    }

    int totalPrice = products[choice - 1].price * quantity;
    clearScreen();
    cout << "> Product Information <" << endl;
    cout << "Product: " << products[choice - 1].name << endl;
    cout << "Descriptioni: " << products[choice - 1].description << endl;
    cout << "Stock: " << products[choice - 1].price << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Unit Price: "
         << FormatHelper::displayCurrency(products[choice - 1].price) << endl;
    cout << "\nTotal Payment: " << FormatHelper::displayCurrency(totalPrice)
         << endl;

    if (InputValidator::validateConfirmation("Confirm purchase? (Y/N): ")) {
        if (shopService.purchaseProduct(userService.getCurrentUser()->id,
                                        products[choice - 1].id, quantity)) {
            cout << "\nPurchase successful!" << endl;
            pause();
        } else {
            cout << "[Error]: Insufficient balance." << endl;
            if (InputValidator::validateConfirmation(
                    "Would you like to top up your balance? (Y/N): ")) {
                topUpBalance();
            } else {
                cout << "Redirecting to menu..." << endl;
                sleep(1);
            }
        }
    } else {
        cout << "Purchase canceled" << endl;
        sleep(1);
    }
}

void ShoppingMenu::topUpBalance() {
    clearScreen();
    cout << userService.getCurrentUser()->fullName << endl;
    cout << "> Top Up Balance <\n" << endl;
    cout << "Current Balance: "
         << FormatHelper::displayCurrency(userService.getCurrentUser()->balance)
         << endl;
    cout << "\nSelect Amount:" << endl;
    cout << "1. Rp. 50.000" << endl;
    cout << "2. Rp. 100.000" << endl;
    cout << "3. Rp. 200.000" << endl;
    cout << "4. Custom Amount" << endl;
    cout << "5. Back" << endl;

    int choice;
    while (true) {
        if (!InputValidator::validateIntInput(choice, "\nEnter choice: "))
            continue;

        int amount;
        switch (choice) {
        case 1:
            amount = 50000;
            return;
        case 2:
            amount = 100000;
            return;
        case 3:
            amount = 200000;
            return;
        case 4:
            if (!InputValidator::validateIntInput(
                    amount, "\nEnter custom amount (minimum Rp.10.000): "))
                continue;
            if (amount < 10000) {
                cout << "[Error]: Minimum top up amount is Rp.10.000." << endl;
                continue;
            }
            return;
        case 5:
            return;
        default:
            cout << "\n[Error]: Invalid option. Please try again." << endl;
            continue;
        }

        if (userService.topUpBalance(userService.getCurrentUser()->id,
                                     amount)) {
            cout << "\nTop up successful!" << endl;
            sleep(0.5);
            cout << "New Balance: "
                 << FormatHelper::displayCurrency(
                        userService.getCurrentUser()->balance)
                 << endl;
            pause();
            return;
        } else {
            cout << "[Error]: Top up failed. Please try again." << endl;
            pause();
            return;
        }
        // TODO Confirmation Top Up
        /* if (InputValidator::validateConfirmation(
                "Confirm top up amount: " +
                FormatHelper::displayCurrency(amount) + " (Y/N): ")) {
            if
            (userService.topUpBalance(userService.getCurrentUser()->id,
                                         amount)) {
                cout << "\nTop up successful!" << endl;
                cout << "New Balance: "
                     << FormatHelper::displayCurrency(
                            userService.getCurrentUser()->balance)
                     << endl;
                pause();
                return;
            } else {
                cout << "\n[Error]: Top up failed. Please try again."
                     << endl;
                pause();
                return;
            }
        } */
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
        cout << "Transaction Date: "
             << FormatHelper::formatDate(transaction.createdAt) << endl;
    }
    cout << endl;
    pause();
}

void ShoppingMenu::updateProfile() {
    clearScreen();
    cout << "> Update Profile <\n" << endl;
    pause();
}