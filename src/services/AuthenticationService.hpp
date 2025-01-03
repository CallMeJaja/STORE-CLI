#pragma once
#include "../repositories/UserRepository.hpp"
#include "string"

class AuthenticationService {
    UserRepository &userRepository;
    User *currentUser;

  public:
    AuthenticationService(UserRepository &userRepository)
        : userRepository(userRepository), currentUser(nullptr) {}

    bool logIn(const string &email, const string &password, const string &pin) {
        auto user = userRepository.findByEmail(email);
        if (!user || user->password != password || user->pin != pin) {
            return false;
        }

        if (!currentUser->isActive) {
            return false;
        }

        currentUser = user;
        return true;
    }

    bool registerUser(const string &fullName, const string &email,
                      const string &password, const string &pin) {
        if (userRepository.findByEmail(email)) {
            return false;
        }

        auto users = userRepository.getUsers();
        int newId = users.empty() ? 1 : users.back().id + 1;

        User newUser(newId, fullName, email, password, pin);
        userRepository.saveUsers(newUser);
        return true;
    }

    void logOut() { currentUser = nullptr; }

    bool isAuthenticated() { return currentUser != nullptr; }

    bool isAdmin() { return currentUser && currentUser->isAdmin; }

    bool validatePassword(const string &password) {
        return password.length() >= 5;
    }

    bool validatePin(const string &pin) {
        return pin.length() == 4 && all_of(pin.begin(), pin.end(), ::isdigit);
    }

    bool validateEmail(const string &email) {
        if (email.empty())
            return false;

        return email.find('@') != string::npos &&
               email.find('.') != string::npos;
    }

    bool updatePassword(const string &currentPassword,
                        const string &newPasswword) {
        if (!currentUser || currentUser->password != currentPassword ||
            !validatePassword(newPasswword)) {
            return false;
        }

        currentUser->password = newPasswword;
        return userRepository.updateUser(*currentUser);
    }

    bool updatePin(const string &currentPin, const string &newPin) {
        if (!currentUser || currentUser->pin != currentPin ||
            !validatePin(newPin)) {
            return false;
        }

        currentUser->pin = newPin;
        return userRepository.updateUser(*currentUser);
    }

    User *getCurrentUser() { return currentUser; }
};