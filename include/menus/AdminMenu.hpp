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
    void manageStores();

    // Product Operation
    void addProduct();
    void updateProduct();
    void deleteProduct();

    // Category Operation
    void addCategory();
    void updateCategory();
    void deleteCategory();

    // User Operation
    void viewUsers();
    void toggleUserAccess();
    void viewReport();

    // Store Operation
    void updateStoreName();
    void updateOwnerName();
    void updateDescription();
    void updatePhoneNumber();
    void viewStoreInfo();

  public:
    AdminMenu(AdminService &adminService, MainMenu *mainMenu);

    void display();
    void displayAllCategories();
    void listUsers();
    void listProducts();
    void listCategories();
};