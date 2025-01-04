#include "MainMenu.hpp"
#include "../services/AdminService.hpp"
#include "../services/AuthenticationService.hpp"
#include "../services/ShoppingService.hpp"
#include "../services/UserService.hpp"
#include "iostream"

using namespace std;

MainMenu::MainMenu(AuthenticationService &auth, ShoppingService &shopping,
                   UserService &user, AdminService &admin)
    : authService(auth), shoppingService(shopping), userService(user),
      adminService(admin) {}

void MainMenu::clearScreen() { system("cls"); }

void MainMenu::handleSignIn() {
    clearScreen();
    cout << "> Sign In to J-STORE <" << endl;

    while (true) {

        if (!InputValidator::validateStringInput(email,
                                                 "\nEnter Email Address: ")) {
            return;
        }

        if (!InputValidator::validateStringInput(password,
                                                 "\nEnter Password: ")) {
            return;
        }

        if (!InputValidator::validateStringInput(pin, "\nEnter PIN: ")) {
            return;
        }

        string status = authService.logIn(email, password, pin);

        if (status == "USER_NOT_FOUND") {
            cout << "[Error]: User not found. Please try again." << endl;
            sleep(1);
            return;
        }

        if (status == "PASSWORD_INCORRECT") {
            cout << "[Error]: Incorrect password. Please try again." << endl;
            sleep(1);
            return;
        }

        if (status == "PIN_INCORRECT") {
            cout << "[Error]: Incorrect PIN. Please try again." << endl;
            sleep(1);
            return;
        }

        if (status == "USER_INACTIVE") {
            cout << "\n[!]: Your account has been deactivated for some "
                    "reason. "
                    "Please contact admin."
                 << endl;
            sleep(1);
            return;
        }

        if (status == "SUCCESS") {
            if (authService.isAdmin()) {
                AdminMenu adminMenu(adminService, this);
                adminMenu.display();
            } else {
                ShoppingMenu shoppingMenu(shoppingService, userService, this);
                shoppingMenu.display();
            }
            authService.logOut();
        }
    }
}

void MainMenu::handleSignUp() {

    clearScreen();
    cout << "> Sign Up to J-STORE <" << endl;

    while (
        !InputValidator::validateStringInput(fullName, "\nEnter Full Name: ")) {
        return;
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
        cout << "\nRegistration successful! Please sign in to continue.";
        sleep(2);
        return;
    } else {
        cout << "[Error]: Email already registered. Please try again.\n";
        sleep(1);
        cout << "Redirecting to Main Menu..." << endl;
        sleep(2);
        return;
    }
}

void MainMenu::displayMainMenu() {
    int choice = 0;
    while (true) {
        clearScreen();
        cout << "> Welcome to J-STORE <" << endl;
        cout << "Your trusted platform for all you needs!\n" << endl;
        cout << "1. Sign In" << endl;
        cout << "2. Sign Up" << endl;
        cout << "3. List Service" << endl;
        cout << "4. Exit" << endl;

        if (!InputValidator::validateIntInput(choice, "Choose an option: ")) {
            // sleep(1);
            continue;
        }

        switch (choice) {
        case 1:
            handleSignIn();
            break;
        case 2:
            handleSignUp();
            break;
        case 3:
            // TODO: REF: Implement listService()
            break;
        case 4:
            cout << "\nThank you for using J-STORE";
            cout << "\nExiting program..." << endl;
            // sleep(1);
            exit(0);
            break;
        default:
            cout << "[Error]: Invalid option. Please try again." << endl;
            sleep(1);
            break;
        }
    }
}