#include "services/AuthenticationService.hpp"
#include "bcrypt/bcrypt.h"
#include "iostream"
#include "utils/InputValidator.hpp"

AuthenticationService::AuthenticationService(UserRepository &userRepository,
                                             UserService &userService)
    : userRepository(userRepository), userService(userService) {}

bool AuthenticationService::logIn(const string &email, const string &password,
                                  const string &pin) {
    auto user = userRepository.findByEmail(email);

    if (!user) {
        cout << "[Error]: User not found. Please try again." << endl;
        return false;
    }

    if (!bcrypt::validatePassword(password, user->password)) {
        cout << "[Error]: Incorrect password. Please try again." << endl;
        return false;
    }

    if (pin != user->pin) {
        cout << "[Error]: Incorrect PIN. Please try again." << endl;
        return false;
    }

    if (!user->isActive) {
        cout << "\n[!]: Your account has been deactivated for some "
                "reason. "
                "Please contact admin."
             << endl;
        return false;
    }

    userService.setCurrentUser(user->id);
    return true;
}

bool AuthenticationService::registerUser(const string &fullName,
                                         const string &email,
                                         const string &password,
                                         const string &pin) {
    if (userRepository.findByEmail(email)) {
        return false;
    }

    auto users = userRepository.getUsers();
    int newId = users.empty() ? 1 : users.back().id + 1;

    string hash = bcrypt::generateHash(password, 10);

    try {
        User newUser(newId, 0, fullName, email, hash, pin, 0, true, false);
        userRepository.saveUsers(newUser);
        return true;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return false;
    }
    return false;
}

void AuthenticationService::logOut() { userService.clearCurrentUser(); }

bool AuthenticationService::isAdmin() {
    return getCurrentUser() && getCurrentUser()->isAdmin;
}

bool AuthenticationService::updatePassword(const string &currentPassword,
                                           const string &newPasswword) {
    if (!getCurrentUser() || getCurrentUser()->password != currentPassword ||
        !InputValidator::validatePassword(newPasswword)) {
        return false;
    }

    getCurrentUser()->password = newPasswword;
    return userRepository.updateUser(*getCurrentUser());
}

bool AuthenticationService::findByEmail(const string &email) {
    auto user = userRepository.findByEmail(email);
    if (!user || user->isAdmin) {
        return false;
    } else {
        return true;
    }
}

bool AuthenticationService::resetPassword(const string &newPassword,
                                          const string &email) {
    auto user = userRepository.findByEmail(email);
    if (!user) {
        return false;
    }

    user->password = bcrypt::generateHash(newPassword, 10);
    return userRepository.updateUser(*user);
}

bool AuthenticationService::resetPin(const string &newPin,
                                     const string &email) {
    auto user = userRepository.findByEmail(email);
    if (!user) {
        return false;
    }

    user->pin = newPin;
    return userRepository.updateUser(*user);
}

bool AuthenticationService::updatePin(const string &currentPin,
                                      const string &newPin) {
    if (!getCurrentUser() || getCurrentUser()->pin != currentPin ||
        !InputValidator::validatePin(newPin)) {
        return false;
    }

    getCurrentUser()->pin = newPin;
    return userRepository.updateUser(*getCurrentUser());
}

shared_ptr<User> AuthenticationService::getCurrentUser() {
    return userService.getCurrentUser();
}