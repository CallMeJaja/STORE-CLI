#pragma once
#include "iostream"
#include "services/AuthenticationService.hpp"
#include "services/ShoppingService.hpp"
#include "services/UserService.hpp"
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

    void handleSignIn() {
        clearScreen();
        cout << "> Sign In to J-STORE <" << endl;

        if (!validateStringInput(email, "\nEnter Email Address: ")) {
            return;
        }
        if (!validateStringInput(password, "\nEnter Password: ")) {
            return;
        }
        if (!validateStringInput(pin, "\nEnter PIN: ")) {
            return;
        }

        // validation and login
        if (!authService.validateEmail(email)) {
            cout << "[Error] Invalid email format\n";
            sleep(1);
            return;
        }

        if (!authService.validatePassword(password)) {
            cout << "[Error] Invalid password format\n";
            sleep(1);
            return;
        }

        if (!authService.validatePin(pin)) {
            cout << "[Error] Invalid PIN format\n";
            sleep(1);
            return;
        }

        if (authService.logIn(email, password, pin)) {
            cout << "\nSign In Successfull!" << endl;
            sleep(1);

            if (authService.isAdmin()) {
                // displayAdminMenu() TODO: Implement AdminMenu
                sleep(1);
            } else {
                // displayUserMenu() TODO: Implement UserMenu
                sleep(1);
            }
        } else {
            cout << "[Error] Incorrect email or password. Please try again.\n";
            sleep(1);
        }
    }

    void handleSignUp() {
        clearScreen();
        cout << "> Sign Up to J-STORE <" << endl;
        if (!validateStringInput(fullName, "\nEnter Full Name: ")) {
            return;
        }
        if (!validateStringInput(email, "\nEnter Email Address: ")) {
            return;
        }
        if (!validateStringInput(password, "\nEnter Password: ")) {
            return;
        }
        if (!validateStringInput(confirmPassword, "\nConfirm Password: ")) {
            return;
        }
        if (!validateStringInput(pin, "\nEnter PIN: ")) {
            return;
        }

        if (password != confirmPassword) {
            cout << "\n[Error]: Password and confirm password do not match. "
                    "Please try again."
                 << endl;
            sleep(1);
            return;
        }

        if (!authService.validateEmail(email)) {
            cout << "[Error]: Invalid email format\n";
            sleep(1);
            return;
        }

        if (!authService.validatePassword(password)) {
            cout << "[Error]: Password must be at least 5 characters\n";
            sleep(1);
            return;
        }

        if (!authService.validatePin(pin)) {
            cout << "[Error]: PIN must be 4 digits\n";
            sleep(1);
            return;
        }

        if (authService.registerUser(fullName, email, password, pin)) {
            cout << "\nRegistration successful! Please sign in to continue.";
            sleep(1);
        } else {
            cout << "[Error]: Email already registered. Please try again.\n";
            sleep(1);
        }
    }

    bool validateIntInput(int &input, const string &prompt) {
        cout << prompt;
        cin >> input;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "-> Invalid input. Please try again." << endl;
            sleep(0.5);
            return false;
        }
        cin.ignore(1000, '\n');
        return true;
    }

    bool validateStringInput(string &input, const string &prompt) {
        while (true) {
            cout << prompt;
            getline(cin, input);
            if (input.empty()) {
                cout << "-> Invalid input. Please try again." << endl;
                sleep(0.5);
            } else {
                return true;
            }
        }
    }

  public:
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

            if (!validateIntInput(choice, "Choose an option: ")) {
                sleep(1);
                continue;
            }

            switch (choice) {
            case 1:
                handleSignIn();
                return;
                // TODO: REF: Implement signIn()
                break;
            case 2:
                handleSignUp();
                return;
                // TODO: REF: Implement signUp()
                break;
            case 3:
                // TODO: REF: Implement listService()
                break;
            case 4:
                cout << "\nThank you for using J-STORE";
                cout << "\nExiting program..." << endl;
                sleep(1);
                exit(0);
                break;
            default:
                cout << "[Error]: Invalid option. Please try again." << endl;
                sleep(1);
                break;
            }
        }
    }
};