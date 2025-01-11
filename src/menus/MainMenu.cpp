#include "menus/MainMenu.hpp"
#include "iostream"
#include "menus/AdminMenu.hpp"
#include "services/AdminService.hpp"
#include "services/AuthenticationService.hpp"
#include "services/ShoppingService.hpp"
#include "services/UserService.hpp"
#include "utils/InputValidator.hpp"

using namespace std;

MainMenu::MainMenu(AuthenticationService &auth, ShoppingService &shopping,
                   UserService &user, AdminService &admin)
    : authService(auth), shoppingService(shopping), userService(user),
      adminService(admin) {}

void MainMenu::clearScreen() { system("cls"); }

void MainMenu::handleSignIn() {
    clearScreen();
    InputValidator::clearInputBuffer();

    cout << "> Sign In to J-STORE <" << endl;
    while (
        !InputValidator::validateStringInput(email,
                                             "\nEnter Email Address: ") ||
        !InputValidator::validateStringInput(password, "\nEnter Password: ") ||
        !InputValidator::validateStringInput(pin, "\nEnter PIN: ")) {
        return;
    }

    if (authService.logIn(email, password, pin)) {
        cout << "\n[Info]: Login successful!" << endl;
        sleep(0.5);
        if (authService.isAdmin()) {
            AdminMenu adminMenu(adminService, this);
            adminMenu.display();
        } else {
            ShoppingMenu shoppingMenu(shoppingService, userService, this);
            shoppingMenu.display();
        }
        authService.logOut();
    } else {
        sleep(2);
    }
}

void MainMenu::handleSignUp() {
    clearScreen();
    InputValidator::clearInputBuffer();
    cout << "> Sign Up to J-STORE <" << endl;

    while (
        !InputValidator::validateStringInput(fullName, "\nEnter Full Name: ")) {
        break;
    }

    while (true) {
        if (!InputValidator::validateStringInput(email,
                                                 "\nEnter Email Address: ")) {
            continue;
            ;
        }

        if (!InputValidator::validateEmail(email)) {
            cout << "[Error]: Invalid email format\n";
            continue;
        }
        break;
    }

    while (true) {
        if (!InputValidator::validateStringInput(password,
                                                 "\nEnter Password: ")) {
            continue;
        }

        if (!InputValidator::validatePassword(password)) {
            cout << "[Error]: Password must be at least 5 characters\n";
            continue;
        }

        if (!InputValidator::validateStringInput(confirmPassword,
                                                 "\nConfirm Password: ")) {
            continue;
        }

        if (password != confirmPassword) {
            cout << "[Error]: Password and confirm password do not "
                    "match. "
                    "Please try again."
                 << endl;
            continue;
        }
        break;
    }

    while (true) {
        if (!InputValidator::validateStringInput(pin, "\nEnter PIN: ")) {
            continue;
        }

        if (!InputValidator::validatePin(pin)) {
            cout << "[Error]: PIN must be 4 digits\n";
            continue;
        }
        break;
    }

    if (authService.registerUser(fullName, email, password, pin)) {
        cout << "\nRegistration kocak successful! Please sign in to continue.";
        sleep(2);
    } else {
        cout << "[Error]: Email already registered. Please try again.\n";
        sleep(1);
        cout << "\nRedirecting to Main Menu..." << endl;
        sleep(2);
    }
}

void MainMenu::displayProducts() {
    AdminMenu adminMenu(adminService, this);
    adminMenu.listProducts();
}

void MainMenu::displayMainMenu() {
    int choice = 0;
    while (true) {
        clearScreen();
        cout << "> Welcome to J-STORE <" << endl;
        cout << "Your trusted platform for all you needs!\n" << endl;
        cout << "1. Sign In" << endl;
        cout << "2. Sign Up" << endl;
        cout << "3. List Services" << endl;
        cout << "4. Exit" << endl;

        while (!InputValidator::validateIntInput(choice,
                                                 "\nChoose an option: ", 4)) {
            break;
        }
        switch (choice) {
        case 1:
            handleSignIn();
            break;
        case 2:
            handleSignUp();
            break;
        case 3:
            displayProducts();
            break;
        case 4:
            cout << "\nThank you for using J-STORE";
            cout << "\nExiting program..." << endl;
            exit(0);
        }
    }
}
