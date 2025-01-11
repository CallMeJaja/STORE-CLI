#include "menus/ShoppingMenu.hpp"
#include "../libs/bcrypt/bcrypt.h"
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

        while (!InputValidator::validateIntInput(choice, "\nEnter choice: ", 5))
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
            updateProfile();
            break;
        case 5:
            cout << "\nSigning out. Please wait..." << endl;
            userService.clearCurrentUser();
            mainMenu->displayMainMenu();
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
        cout << i + 1 << ". " << categories[i].name << endl;
    }
    cout << categories.size() + 1 << ". Back to Menu" << endl;

    int choice;
    while (InputValidator::validateIntInput(
        choice, "\nEnter choice: ", categories.size() + 1)) {
        if (choice == categories.size() + 1) {
            return;
        } else {
            break;
        }
    }

    viewProducts(categories[choice - 1].name);
}

void ShoppingMenu::viewProducts(const string &category) {
    clearScreen();
    // TODO Improve list product
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
    // FIXME Improve logic selection
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
    cout << "Stock: " << products[choice - 1].stock << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Unit Price: "
         << FormatHelper::displayCurrency(products[choice - 1].price) << endl;
    cout << "\nTotal Payment: " << FormatHelper::displayCurrency(totalPrice)
         << endl;

    if (InputValidator::validateConfirmation("Confirm purchase? (Y/N): ")) {
        if (userService.getCurrentUser()->balance < totalPrice) {
            cout << "[Error]: Insufficient balance." << endl;
            if (InputValidator::validateConfirmation(
                    "Would you like to top up your balance? (Y/N): ")) {
                topUpBalance();
            } else {
                cout << "Redirecting to menu..." << endl;
                sleep(1);
            }
        } else if (shopService.purchaseProduct(userService.getCurrentUser()->id,
                                               products[choice - 1].id,
                                               quantity)) {
            cout << "\nPurchase successful!" << endl;
            pause();
        } else {
            cout << "\n[Error]: Purchase failed. Please try again." << endl;
            pause();
        }
    } else {
        cout << "Purchase canceled" << endl;
        sleep(1);
    }
}

void ShoppingMenu::topUpBalance() {
    clearScreen();
    int amount;
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

    while (InputValidator::validateIntInput(choice, "\nEnter choice: ", 5)) {
        if (choice == 4) {
            while (InputValidator::validateIntInput(
                amount, "\nEnter custom amount (minimum Rp.10.000): ")) {
                if (amount > 1000000) {
                    cout << "[Error]: Maximum top up amount is Rp.1.000.000."
                         << endl;
                    continue;
                } else {
                    break;
                }

                if (amount >= 10000) {
                    break;
                } else {
                    cout << "[Error]: Minimum top up amount is Rp.10.000."
                         << endl;
                    continue;
                }
            }
            break;
        } else {
            break;
        }
    }

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
    }

    if (InputValidator::validateConfirmation(
            "\nConfirm top up amount: " +
            FormatHelper::displayCurrency(amount) + " (Y/N): ")) {
        if (userService.topUpBalance(userService.getCurrentUser()->id,
                                     amount)) {
            cout << "\nTop up successful!" << endl;
            cout << "New Balance: "
                 << FormatHelper::displayCurrency(
                        userService.getCurrentUser()->balance)
                 << endl;
            pause();

        } else {
            cout << "\n[Error]: Top up failed. Please try again." << endl;
            pause();
        }
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
    string newName, newEmail, newPin, newPass, oldPass;
    auto user = userService.getCurrentUser();

    cout << "> Update Profile <\n" << endl;
    cout << "Current Name\t: " << user->fullName << endl;
    cout << "Current Email\t: " << user->email << endl;
    cout << "Current PIN\t: " << user->pin << endl;
    cout << string(50, '=') << endl;

    cout << "1. Update Name" << endl;
    cout << "2. Update Email" << endl;
    cout << "3. Update PIN" << endl;
    cout << "4. Update Password" << endl;
    cout << "5. Back" << endl;

    int choice;
    while (!InputValidator::validateIntInput(choice, "\nEnter choice: ", 5))
        continue;

    InputValidator::clearInputBuffer();
    switch (choice) {
    case 1:
        while (!InputValidator::validateStringInput(newName, "\nNew Name: ")) {
            continue;
        }
        user->fullName = newName;
        break;
    case 2:
        while (InputValidator::validateStringInput(newEmail, "\nNew Email: ")) {
            if (InputValidator::validateEmail(newEmail)) {
                user->email = newEmail;
                break;
            } else {
                cout << "[Error]: Invalid email format. Please try again."
                     << endl;
                continue;
            }
        }
        break;
    case 3:
        while (InputValidator::validateStringInput(newPin, "\nNew PIN: ")) {
            if (InputValidator::validatePin(newPin)) {
                user->pin = newPin;
                break;
            } else {
                cout << "[Error]: Invalid PIN format. PIN must be 4 digits. "
                        "Please try again"
                     << endl;
                continue;
            }
        }
        break;
    case 4:
        while (
            InputValidator::validateStringInput(oldPass, "\nOld Password: ")) {
            if (bcrypt::validatePassword(oldPass, user->password)) {
                break;
            } else {
                cout << "[Error]: Incorrect password. Please try again."
                     << endl;
                continue;
            }
        }
        while (
            InputValidator::validateStringInput(newPass, "\nNew Password: ")) {
            if (InputValidator::validatePassword(newPass)) {
                user->password = bcrypt::generateHash(newPass);
                break;
            } else {
                cout << "[Error]: Password must be at least 5 characters. "
                        "Please try again"
                     << endl;
                continue;
            }
        }
        break;
    case 5:
        return;
    }

    if (userService.updateUser(*user)) {
        cout << "\nProfile updated successfully!" << endl;
    } else {
        cout << "\n[Error]: Failed to update profile. Please try again."
             << endl;
    }
    pause();
}