#pragma once
#include "../services/AdminService.hpp"
#include "../utils/InputValidator.hpp"
#include "MenuBase.hpp"
#include <unistd.h>

class AdminMenu : public MenuBase {
  private:
    AdminService &adminService;

    void manageProducts() {
        while (true) {
            clearScreen();
            cout << "> Manage Products <" << endl;
            cout << "\n1. Add Product" << endl;
            cout << "2. Update Product" << endl;
            cout << "3. Delete Product" << endl;
            cout << "4. Back" << endl;

            int choice;
            if (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
                continue;
            }

            switch (choice) {
            case 1:
                /* addProducts */
                break;
            case 2:
                /* updateProducts */
                break;
            case 3:
                /* deleteProducts */
                break;
            case 4:
                return;
            default:
                break;
            }
        }
    }

  public:
    AdminMenu(AdminService &adminService) : adminService(adminService) {}

    void display() {
        while (true) {
            clearScreen();
            cout << "> Admin Menu <" << endl;
            cout << "\n1. Manage Products" << endl;
            cout << "2. Manage Categories" << endl;
            cout << "3. Manage Users" << endl;
            cout << "4. View Reports" << endl;
            cout << "5. Sign Out" << endl;

            int choice;
            if (!InputValidator::validateIntInput(choice, "\nEnter choice: ")) {
                continue;
            }

            switch (choice) {
            case 1:
                /* TODO: manageProducts() */
                break;
            case 2:
                /* TODO: manageCategories() */
                break;
            case 3:
                /* TODO: manageUsers() */
                break;
            case 4:
                /* TODO: viewReport */
                break;
            case 5:
                return;
            default:
                cout << "[Error]: Invalid option. Please try again." << endl;
                sleep(1);
            }
        }
    }

    void addProduct() {
        clearScreen();
        cout << "> Add Product <\n" << endl;
        string name, description;
        int price, stock;
        vector<string> categories = {"All Products"};

        if (!InputValidator::validateStringInput(name,
                                                 "Enter product name: ")) {
            return;
        }

        if (!InputValidator::validateIntInput(price, "Enter product price: ")) {
            return;
        }

        if (!InputValidator::validateStringInput(
                description, "Enter product description: ")) {
            return;
        }

        if (!InputValidator::validateIntInput(stock, "Enter product stock: ")) {
            return;
        }

        // category selection
        auto availableCategories = adminService.getCategories();
        if (!availableCategories.empty()) {
            cout << "\nAvailable Categories:" << endl;
            for (int i = 0; i < availableCategories.size(); i++) {
                cout << i + 1 << ". " << availableCategories[i].name << endl;
            }

            int choice;
            if (!InputValidator::validateIntInput(
                    choice, "Select additional category (0 to skip): ")) {
                return;
            }

            if (choice > 0 && choice <= availableCategories.size()) {
                categories.push_back(availableCategories[choice - 1].name);
            }
        }

        if (adminService.addProduct(name, price, description, categories,
                                    stock)) {
            cout << "\nProduct added successfully!" << endl;
        } else {
            cout << "\n[Error]: Failed to add product. Please try again."
                 << endl;
        }
        pause();
    }
};