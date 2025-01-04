#pragma once
#include "algorithm"
#include "iostream"
#include "string"
#include "unistd.h"

using namespace std;

class InputValidator {
  public:
    static bool validateStringInput(string &input, const string &prompt) {
        while (true) {
            cout << prompt;
            getline(cin, input);
            if (input.empty()) {
                cout << "[Error] Invalid input. Please try again." << endl;
                sleep(0.5);
            } else {
                return true;
            }
        }
    }

    static bool validateIntInput(int &input, const string &prompt) {
        cout << prompt;
        cin >> input;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[Error] Invalid input. Please try again." << endl;
            sleep(0.5);
            return false;
        }
        cin.ignore(1000, '\n');
        return true;
    }

    static bool validatePassword(const string &password) {
        return password.length() >= 5;
    }

    static bool validatePin(const string &pin) {
        return pin.length() == 4 && all_of(pin.begin(), pin.end(), ::isdigit);
    }

    static bool validateEmail(const string &email) {
        if (email.empty())
            return false;

        return email.find('@') != string::npos &&
               email.find('.') != string::npos;
    }

    static bool validateConfirmation(const string &prompt) {
        char confirmation;
        cout << prompt;
        cin >> confirmation;
        cin.ignore();
        return tolower(confirmation) == 'y';
    }
};