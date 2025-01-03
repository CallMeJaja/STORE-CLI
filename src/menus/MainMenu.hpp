#pragma once
#include "../services/AuthenticationService.hpp"
#include "../services/ShoppingService.hpp"
#include "../services/UserService.hpp"
#include "../utils/InputValidator.hpp"
#include "iostream"
#include "string"
#include "unistd.h"

using namespace std;

class MainMenu {
  private:
    AuthenticationService &authService;
    ShoppingService &shoppingService;
    UserService &userService;
    string fullName, email, password, confirmPassword, pin;

    void clearScreen() { system("cls"); }

    void stp() { system("cls"); }

    void handleSignIn() {
        clearScreen();
        cout << "> Sign In to J-STORE <" << endl;

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
        // FIXME: FIX LOGIC SIGNIN
        //  if (authService.logIn(email, password, pin)) {
        //      cout << "\nSign In Successfull!" << endl;
        //      // sleep(1);

        //     if (authService.isAdmin()) {
        //         cout << "ADMIN MENU\n";
        //         system("pause");
        //         // displayAdminMenu() TODO: Implement AdminMenu
        //         // sleep(1);
        //     } else {
        //         cout << "USER MENU\n";
        //         system("pause");
        //         // displayUserMenu() TODO: Implement UserMenu
        //         // sleep(1);
        //     }
        // } else {
        //     cout << "[Error] Incorrect email or password. Please try
        //     again.\n"; sleep(2);
        // }
    }

    void handleSignUp() {
        clearScreen();
        cout << "> Sign Up to J-STORE <" << endl;

        while (!InputValidator::validateStringInput(fullName,
                                                    "\nEnter Full Name: ")) {
            return;
        }

        while (true) {
            if (!InputValidator::validateStringInput(
                    email, "\nEnter Email Address: ")) {
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
            // TODO: Route to displaymenu
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

  public:
    MainMenu(AuthenticationService &auth, ShoppingService &shopping,
             UserService &user)
        : authService(auth), shoppingService(shopping), userService(user) {}

    void displayMainMenu() {
        int choice = 0;
        while (true) {
            clearScreen();
            cout << "> Welcome to J-STORE <" << endl;
            cout << "Your trusted platform for all you needs!\n" << endl;
            cout << "1. Sign In" << endl;
            cout << "2. Sign Up" << endl;
            cout << "3. List Service" << endl;
            cout << "4. Exit" << endl;

            if (!InputValidator::validateIntInput(choice,
                                                  "Choose an option: ")) {
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
                // sleep(1);
                break;
            }
        }
    }
};