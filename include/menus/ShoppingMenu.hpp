#pragma once

#include "MainMenu.hpp"
#include "MenuBase.hpp"
#include "services/AdminService.hpp"
#include "services/ShoppingService.hpp"
#include "services/UserService.hpp"
#include "string"
#include "utils/FormatHelper.hpp"
#include "utils/InputValidator.hpp"

class MainMenu;

class ShoppingMenu : public MenuBase {
  private:
    ShoppingService &shopService;
    UserService &userService;
    MainMenu *mainMenu;

  public:
    ShoppingMenu(ShoppingService &shopService, UserService &userService,
                 MainMenu *mainMenu);
    void display();
    void browseCategories();
    void viewProducts(const string &category);
    void topUpBalance();
    void updateProfile();
    void viewTransactionHistory();
};