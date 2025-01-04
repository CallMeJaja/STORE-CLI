#include "menus/MainMenu.hpp"
#include "repositories/CategoryRepository.hpp"
#include "repositories/ProductRepository.hpp"
#include "repositories/TransactionRepository.hpp"
#include "repositories/UserRepository.hpp"
#include <filesystem>

int main() {
    // Define data file paths
    const string DATA_PATH = "data/";
    filesystem::create_directory(DATA_PATH);

    const string userPath = DATA_PATH + "users.json";
    const string productPath = DATA_PATH + "products.json";
    const string transactionPath = DATA_PATH + "transactions.json";
    const string categoryPath = DATA_PATH + "categories.json";

    // Initialize repositories with file paths
    UserRepository userRepo(userPath);
    ProductRepository productRepo(productPath);
    TransactionRepository transactionRepo(transactionPath);
    CategoryRepository categoryRepo(categoryPath);

    // Initialize services
    AuthenticationService authService(userRepo);
    TransactionService transactionService(transactionRepo, productRepo,
                                          userRepo);
    ShoppingService shoppingService(productRepo, userRepo, transactionService,
                                    categoryRepo);
    ProductService productService(productRepo, categoryRepo);
    CategoryService categoryService(categoryRepo);
    UserService userService(userRepo, transactionRepo, authService);
    AdminService admin(userService, transactionService, productService,
                       categoryService);

    // Start application
    MainMenu menu(authService, shoppingService, userService, admin);
    menu.displayMainMenu();
    return 0;
}