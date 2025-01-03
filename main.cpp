#include "src/MainMenu.hpp"
#include "src/repositories/CategoryRepository.hpp"
#include "src/repositories/ProductRepository.hpp"
#include "src/repositories/TransactionRepository.hpp"
#include "src/repositories/UserRepository.hpp"
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
    UserService userService(userRepo, transactionRepo);

    // Start application
    MainMenu menu(authService, shoppingService, userService);
    menu.displayMainMenu();
    return 0;
}