#pragma once

#include "MainMenu.hpp"
#include "ShoppingMenu.hpp"
#include "iostream"
#include "services/AdminService.hpp"
#include "services/AuthenticationService.hpp"
#include "services/ShoppingService.hpp"
#include "services/StoreService.hpp"
#include "services/UserService.hpp"
#include "string"
#include "unistd.h"
#include "utils/InputValidator.hpp"

using namespace std;

class ShoppingMenu;

class MainMenu {
  private:
    AuthenticationService &authService;
    ShoppingService &shoppingService;
    UserService &userService;
    AdminService &adminService;
    StoreService &storeService;

    string fullName, email, password, confirmPassword, pin;

    void clearScreen();
    void handleSignIn();
    void handleSignUp();
    void handleForgotPassword();
    void handleForgotPin(const string &email);

  public:
    MainMenu(AuthenticationService &auth, ShoppingService &shopping,
             UserService &user, AdminService &admin, StoreService &store);

    void displayMainMenu();
    void displayProducts();

    // Store Management
    vector<Store> viewStoreInfo();
    bool updateStore(Store &store);
};