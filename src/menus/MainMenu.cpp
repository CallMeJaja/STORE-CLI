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
            cout << "[Error]: Invalid email format. Please try again.\n";
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
            cout << "[Error]: Invalid PIN format. PIN must be 4 digits. Please "
                    "try again.\n";
            continue;
        }
        break;
    }

    if (authService.registerUser(fullName, email, password, pin)) {
        cout << "\nRegistration successful! Please sign in to continue.";
        sleep(2);
    } else {
        cout << "[Error]: Email already registered. Please try again.\n";
        sleep(1);
        cout << "\nRedirecting to Main Menu..." << endl;
        sleep(2);
    }
}

void MainMenu::handleForgotPin(const string &email) {
    InputValidator::clearInputBuffer();
    string newPin;
    while (InputValidator::validateStringInput(newPin, "\nEnter New PIN: ")) {
        if (InputValidator::validatePin(newPin)) {
            break;
        } else {
            cout << "[Error]: Invalid PIN format. PIN must be 4 digits. Please "
                    "try again.\n";
            continue;
        }
    }

    if (authService.resetPin(newPin, email)) {
        cout << "\n[Info]: PIN reset successful!. Please sign in to "
                "continue";
        sleep(2);
    } else {
        cout << "[Error]: PIN reset failed. Please try again.\n ";
        sleep(1);
    }
}

void MainMenu::handleForgotPassword() {
    clearScreen();
    InputValidator::clearInputBuffer();
    string email, password;
    cout << "> Forgot Password <" << endl;

    while (InputValidator::validateStringInput(email, "\nEnter Your Email: ")) {
        if (authService.findByEmail(email)) {
            break;
        } else {
            cout << "[Error]: Email not found. Please try again.\n";
            continue;
        }
    }

    while (InputValidator::validateStringInput(password,
                                               "\nEnter New Password: ")) {
        if (InputValidator::validatePassword(password)) {
            break;
        } else {
            cout << "[Error]: Password must be at least 5 characters. "
                    "Please try again.\n";
            continue;
        }
    }

    if (authService.resetPassword(password, email)) {
        cout << "[Info]: Password reset successful!" << endl;
        if (InputValidator::validateConfirmation("\nWould you like to reset "
                                                 "your PIN as well? (y/n): ")) {
            handleForgotPin(email);
        } else {
            cout << "\nRedirecting to Main Menu..." << endl;
            sleep(2);
        }
    } else {
        cout << "[Error]: Password reset failed. Please try again.\n ";
        sleep(1);
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
        cout << "4. Forgot Password & PIN" << endl;
        cout << "5. Contact Us" << endl;
        cout << "6. Exit" << endl;

        while (!InputValidator::validateIntInput(choice,
                                                 "\nChoose an option: ", 6)) {
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
            handleForgotPassword();
            break;
        case 5:
            system("start https://s.id/CallMeJajaWA");
            break;
        case 6:
            cout << "\nThank you for using J-STORE";
            cout << "\nExiting program..." << endl;
            exit(0);
        }
    }
}
