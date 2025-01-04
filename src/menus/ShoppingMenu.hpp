#pragma once
#include "../services/ShoppingService.hpp"
#include "../services/UserService.hpp"
#include "../utils/FormatHelper.hpp"
#include "../utils/InputValidator.hpp"
#include "MainMenu.hpp"
#include "MenuBase.hpp"
#include "string"

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

    void viewTransactionHistory();
};