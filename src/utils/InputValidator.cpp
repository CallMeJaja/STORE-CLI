#include "utils/InputValidator.hpp"

bool InputValidator::validateStringInput(string &input, const string &prompt) {
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            FormatHelper::handleInvalidInput();
            sleep(0.5);
            continue;
        } else {
            return true;
        }
    }
}

bool InputValidator::validateIntInput(int &input, const string &prompt,
                                      int menuSize) {
    while (true) {
        cout << prompt;
        cin >> input;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            FormatHelper::handleInvalidInput();
            sleep(0.5);
        } else {
            if (menuSize > 0) {
                if (input < 0 || input > menuSize) {
                    FormatHelper::handleInvalidOption();
                    sleep(0.5);
                    continue;
                } else {
                    sleep(0.5);
                    return true;
                }
            } else {
                cin.ignore(1000, '\n');
                return true;
            }
        }
    }
}

bool InputValidator::validatePassword(const string &password) {
    return password.length() >= 5;
}

bool InputValidator::validatePin(const string &pin) {
    return pin.length() == 4 && all_of(pin.begin(), pin.end(), ::isdigit);
}

bool InputValidator::validateEmail(const string &email) {
    return email.find('@') != string::npos && email.find('.') != string::npos;
    ;
}

bool InputValidator::validateConfirmation(const string &prompt) {
    char input;
    while (true) {
        cout << prompt;
        cin >> input;
        if (input == 'y' || input == 'Y') {
            return true;
        } else if (input == 'n' || input == 'N') {
            return false;
        } else {
            FormatHelper::handleInvalidInput();
            sleep(0.5);
            continue;
        }
    }
}

void InputValidator::clearInputBuffer() {
    cin.clear();
    cin.ignore(1000, '\n');
}
