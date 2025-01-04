#pragma once
#include "../repositories/UserRepository.hpp"
#include "../utils/InputValidator.hpp"
#include "memory"
#include "string"

class AuthenticationService {
    UserRepository &userRepository;
    shared_ptr<User> currentUser;

    // void initializeDefaultAdmin() {
    //     auto users = userRepository.getUsers();
    //     User admin(1, "Admin", "admin@admin.com", "admin", "1111");
    //     admin.isAdmin = true;
    //     admin.isActive = true;
    //     userRepository.saveUsers(admin);
    // }

  public:
    AuthenticationService(UserRepository &userRepository)
        : userRepository(userRepository), currentUser(nullptr) {
        // initializeDefaultAdmin();
    }

    string logIn(const string &email, const string &password,
                 const string &pin) {
        auto user = userRepository.findByEmail(email);

        if (!user)
            return "USER_NOT_FOUND";

        if (password != user->password)
            return "PASSWORD_INCORRECT";

        if (pin != user->pin)
            return "PIN_INCORRECT";

        if (!user->isActive)
            return "USER_INACTIVE";

        currentUser = user;

        return "SUCCESS";
    }

    bool registerUser(const string &fullName, const string &email,
                      const string &password, const string &pin) {
        if (userRepository.findByEmail(email)) {
            return false;
        }

        auto users = userRepository.getUsers();
        int newId = users.empty() ? 1 : users.back().id + 1;

        User newUser(newId, fullName, email, password, pin);
        newUser.isActive = true;
        newUser.balance = 0;
        userRepository.saveUsers(newUser);
        return true;
    }

    void logOut() { currentUser = nullptr; }

    bool isAuthenticated() { return currentUser != nullptr; }

    bool isAdmin() { return currentUser && currentUser->isAdmin; }

    bool updatePassword(const string &currentPassword,
                        const string &newPasswword) {
        if (!currentUser || currentUser->password != currentPassword ||
            !InputValidator::validatePassword(newPasswword)) {
            return false;
        }

        currentUser->password = newPasswword;
        return userRepository.updateUser(*currentUser);
    }

    bool updatePin(const string &currentPin, const string &newPin) {
        if (!currentUser || currentUser->pin != currentPin ||
            !InputValidator::validatePin(newPin)) {
            return false;
        }

        currentUser->pin = newPin;
        return userRepository.updateUser(*currentUser);
    }

    shared_ptr<User> getCurrentUser() { return currentUser; }
};