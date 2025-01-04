#include "AuthenticationService.hpp"
#include "../utils/InputValidator.hpp"
#include "iostream"

AuthenticationService::AuthenticationService(UserRepository &userRepository,
                                             UserService &userService)
    : userRepository(userRepository), userService(userService) {}

string AuthenticationService::logIn(const string &email, const string &password,
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

    userService.setCurrentUser(user->id);

    return "SUCCESS";
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

    User newUser(newId, fullName, email, password, pin);
    newUser.isActive = true;
    newUser.balance = 0;
    userRepository.saveUsers(newUser);
    return true;
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