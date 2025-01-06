#pragma once
#include "UserService.hpp"
#include "memory"
#include "repositories/UserRepository.hpp"
#include "string"
#include "utils/InputValidator.hpp"

class UserService;

class AuthenticationService {
    UserRepository &userRepository;
    UserService &userService;

  public:
    AuthenticationService(UserRepository &userRepository,
                          UserService &userService);

    string logIn(const string &email, const string &password,
                 const string &pin);

    bool registerUser(const string &fullName, const string &email,
                      const string &password, const string &pin);

    void logOut();

    bool isAdmin();

    bool updatePassword(const string &currentPassword,
                        const string &newPasswword);

    bool updatePin(const string &currentPin, const string &newPin);

    shared_ptr<User> getCurrentUser();
};