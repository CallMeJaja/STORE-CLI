#pragma once
#include "MainMenu.hpp"
#include "MenuBase.hpp"
#include "services/AdminService.hpp"
#include "utils/FormatHelper.hpp"
#include "utils/InputValidator.hpp"
#include <unistd.h>

class MainMenu;

class AdminMenu : public MenuBase {
  private:
    AdminService &adminService;
    MainMenu *mainMenu;

    // Management Methods
    void manageProducts();
    void manageCategories();
    void manageUsers();

    // Product Operation
    void listProducts();
    void addProduct();
    void updateProduct();
    void deleteProduct();

    // Category Operation
    void listCategories();
    void addCategory();
    void updateCategory();
    void deleteCategory();

    // User Operation
    void viewUsers();
    void toggleUserAccess();
    void viewReport();

  public:
    AdminMenu(AdminService &adminService, MainMenu *mainMenu);

    void display();
    void displayAllProducts();
    void displayAllCategories();
};