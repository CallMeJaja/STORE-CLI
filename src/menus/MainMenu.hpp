#pragma once
#include "../services/AdminService.hpp"
#include "../services/AuthenticationService.hpp"
#include "../services/ShoppingService.hpp"
#include "../services/UserService.hpp"
#include "../utils/InputValidator.hpp"
#include "AdminMenu.hpp"
#include "ShoppingMenu.hpp"
#include "iostream"
#include "string"
#include "unistd.h"

using namespace std;

class ShoppingMenu;

class MainMenu {
  private:
    AuthenticationService &authService;
    ShoppingService &shoppingService;
    UserService &userService;
    AdminService &adminService;
    string fullName, email, password, confirmPassword, pin;

    void clearScreen();
    void handleSignIn();
    void handleSignUp();

  public:
    MainMenu(AuthenticationService &auth, ShoppingService &shopping,
             UserService &user, AdminService &admin);

    void displayMainMenu();
};