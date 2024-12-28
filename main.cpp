#include "fstream"
#include "iomanip"
#include "iostream"
#include "nlohmann/json.hpp"
#include "unistd.h"
#include "vector"
#define FILE_PATH_PRODUCTS "data/products.json"
#define FILE_PATH_CATEGORIES "data/categories.json"
#define FILE_PATH_TRANSACTIONS "data/transactions.json"
#define FILE_PATH_USERS "data/users.json"

using namespace std;
using json = nlohmann::json;

class JSONUtility {
  private:
    string filePath;

    json readJSON() {
        ifstream file(filePath);
        json data;
        if (file.is_open()) {
            file >> data;
            file.close();
        }
        return data;
    }

    void writeJSON(const json &data) {
        ofstream file(filePath);
        if (file.is_open()) {
            file << data.dump(4);
            file.close();
        }
    }

  public:
    JSONUtility(const string &path) : filePath(path) {}

    json getData() { return readJSON(); }

    void saveData(const json &data) { writeJSON(data); }

    static string displayCurrency(double amount) {
        stringstream ss;
        ss << fixed << setprecision(0) << amount;

        string result = ss.str();
        int inseretPosition = result.length() - 3;

        while (inseretPosition > 0) {
            result.insert(inseretPosition, ".");
            inseretPosition -= 3;
        }

        return "Rp. " + result;
    }

    static bool validateIntInput(int &input, const string &prompt) {
        while (true) {
            cout << prompt;
            cin >> input;
            if (cin.fail() || input < 0) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "-> Invalid input. Please try again." << endl;
                sleep(0.5);
            } else {
                cin.ignore(1000, '\n');
                return true;
            }
        }
    }

    static bool validateStringInput(string &input, const string &prompt) {
        while (true) {
            cout << prompt;
            getline(cin, input);
            if (input.empty()) {
                cout << "-> Invalid input. Please try again." << endl;
                sleep(0.5);
            } else {
                return true;
            }
        }
    }
};

class User {
  public:
    int id;
    int balance;
    string fullName;
    string email;
    string password;
    string pin;

    User(int id, int balance, string fullName, string email, string password,
         string pin)
        : id(id), balance(balance), fullName(fullName), email(email),
          password(password), pin(pin) {}

    static vector<User> loadUsers(const string &filePath) {
        JSONUtility service(filePath);
        json data = service.getData();
        vector<User> users;
        for (const auto &item : data) {
            users.emplace_back(item["id"], item["balance"], item["fullName"],
                               item["email"], item["password"], item["pin"]);
        }
        return users;
    }

    static void saveUsers(const vector<User> &users, const string &filePath) {
        JSONUtility service(filePath);
        json data;
        for (const auto &user : users) {
            data.push_back({
                {"id", user.id},
                {"balance", user.balance},
                {"fullName", user.fullName},
                {"email", user.email},
                {"password", user.password},
                {"pin", user.pin},
            });
            service.saveData(data);
        }
    }
    void topUpBalance(int amount) { balance += amount; }
};

class Product {
  public:
    int id;
    int price;
    int stock;
    string name;
    string description;
    string category;

    Product(int id, const string &name, int price, const string &description,
            const string &category, int stock)
        : id(id), name(name), price(price), description(description),
          category(category), stock(stock) {}

    static vector<Product> loadProducts(const string &filePath) {
        JSONUtility service(filePath);
        json data = service.getData();
        vector<Product> products;
        for (const auto &item : data) {
            products.emplace_back(item["id"], item["name"], item["price"],
                                  item["description"], item["category"],
                                  item["stock"]);
        }

        return products;
    }

    static void saveProducts(const vector<Product> &products,
                             const string &filePath) {
        JSONUtility service(filePath);
        json data;
        for (const auto &product : products) {
            data.push_back({{"id", product.id},
                            {"name", product.name},
                            {"price", product.price},
                            {"description", product.description},
                            {"category", product.category},
                            {"stock", product.stock}});
        }
        service.saveData(data);
    }
};

class Category {
  public:
    int id;
    string name;

    Category(int id, const string &name) : id(id), name(name) {}

    static vector<Category> loadCategories(const string &filePath) {
        JSONUtility service(filePath);
        json data = service.getData();
        vector<Category> categories;
        for (const auto &item : data) {
            categories.emplace_back(item["id"], item["name"]);
        }
        return categories;
    }

    static void saveCategories(const vector<Category> &categories,
                               const string &filePath) {
        JSONUtility service(filePath);
        json data;
        for (const auto &category : categories) {
            data.push_back({{"id", category.id}, {"name", category.name}});
        }
        service.saveData(data);
    }
};

class Transaction {
  public:
    int transactionId;
    int userId;
    int productId;
    string productName;
    int quantity;
    int totalPayment;

    Transaction(int transactionId, int userId, int productId,
                const string &productName, int quantity, int totalPayment)
        : transactionId(transactionId), userId(userId), productId(productId),
          productName(productName), quantity(quantity),
          totalPayment(totalPayment) {}

    static vector<Transaction> loadTransactions(const string &filePath) {
        JSONUtility service(filePath);
        json data = service.getData();
        vector<Transaction> transactions;
        for (const auto &item : data) {
            transactions.emplace_back(item["transactionId"], item["userId"],
                                      item["productId"], item["productName"],
                                      item["quantity"], item["totalPayment"]);
        }
        return transactions;
    }

    static void saveTransactions(const vector<Transaction> &transactions,
                                 const string &filePath) {
        JSONUtility service(filePath);
        json data;
        for (const auto &transaction : transactions) {
            data.push_back({{"transactionId", transaction.transactionId},
                            {"userId", transaction.userId},
                            {"productId", transaction.productId},
                            {"productName", transaction.productName},
                            {"quantity", transaction.quantity},
                            {"totalPayment", transaction.totalPayment}});
        }
        service.saveData(data);
    }

    static int calculateTotalRevenue(const vector<Transaction> &transactions) {
        int totalRevenue = 0;
        for (const auto &transaction : transactions) {
            totalRevenue += transaction.totalPayment;
        }
        return totalRevenue;
    }

    static int
    calculateTotalTransactions(const vector<Transaction> &transactions) {
        return transactions.size();
    }
};

class Admin {
  public:
    static vector<Product> loadProducts() {
        return Product::loadProducts(FILE_PATH_PRODUCTS);
    }

    static vector<Category> loadCategory() {
        return Category::loadCategories(FILE_PATH_CATEGORIES);
    }

    static vector<Transaction> loadTransaction() {
        return Transaction::loadTransactions(FILE_PATH_TRANSACTIONS);
    }

    static vector<User> loadUsers() { return User::loadUsers(FILE_PATH_USERS); }

    static void displayCategories(const string &filePath) {
        vector<Category> categories = loadCategory();
        cout << "\nLIST OF CATEGORIES" << endl;
        if (categories.empty()) {
            cout << "No categories available." << endl;
            return;
        }
        for (const auto &category : categories) {
            cout << category.id << ". " << category.name << endl;
        }
    }

    static void displayProducts(const string &filePath) {
        vector<Product> products = loadProducts();
        cout << "\nLIST OF PRODUCTS" << endl;
        if (products.empty()) {
            cout << "No products available." << endl;
            return;
        }
        for (const auto &product : products) {
            cout << product.id << ". " << product.name << " | "
                 << product.category << " | " << product.price << endl;
        }
    }

    static void displayProductsByCategory(const vector<Product> &products,
                                          const string &category) {

        system("cls");
        cout << "> List of Products In " << category << " <\n" << endl;
        bool found = false;
        for (const auto &product : products) {
            if (product.category == category) {
                cout << product.id << ". " << product.name << " -> "
                     << product.stock << " Stock" << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "No products available in this category." << endl;
        }
    }

    static void addProduct(const string &filePath) {
        vector<Product> products = loadProducts();
        vector<Category> categories = loadCategory();

        int id = products.empty() ? 1 : products.back().id + 1;
        int price, categoryId, stock;
        string name, description, category;

        system("cls");
        cout << "> Add Product <" << endl;
        displayCategories(FILE_PATH_CATEGORIES);

        if (categories.empty()) {
            sleep(2);
            return;
        }

        while (true) {
            if (!JSONUtility::validateIntInput(
                    categoryId, "\nEnter Category ID (0 = Back): ")) {
                return;
            }

            if (categoryId == 0) {
                return;
            }

            auto it = find_if(categories.begin(), categories.end(),
                              [categoryId](const Category &category) {
                                  return category.id == categoryId;
                              });

            if (it == categories.end()) {
                cout << "-> Invalid Category ID." << endl;
                sleep(0.5);
                continue;
            } else {
                category = it->name;
                break;
            }
        }

        while (!JSONUtility::validateStringInput(name,
                                                 "\nEnter Product Name : ")) {
            return;
        }

        while (
            !JSONUtility::validateIntInput(price, "\nEnter Product Price : ")) {
            return;
        }

        while (
            !JSONUtility::validateIntInput(stock, "\nEnter Product Stock : ")) {
            return;
        }

        while (!JSONUtility::validateStringInput(
            description, "\nEnter Product Description : ")) {
            return;
        }

        products.emplace_back(id, name, price, description, category, stock);
        Product::saveProducts(products, filePath);
        cout << "\nProduct added successfully." << endl;
        sleep(2);
    }

    static void editProduct(const string &filePath) {
        vector<Product> products = loadProducts();
        vector<Category> categories = loadCategory();

        int productId, categoryId, price, stock;
        string name, description, category;

        system("cls");
        cout << "> Update Product <" << endl;
        displayCategories(FILE_PATH_CATEGORIES);

        if (categories.empty()) {
            sleep(2);
            return;
        }

        while (true) {
            if (!JSONUtility::validateIntInput(
                    categoryId, "\nEnter Category ID (0 = Back): ")) {
                return;
            }

            if (categoryId == 0) {
                return;
            }

            auto it = find_if(categories.begin(), categories.end(),
                              [categoryId](const Category &category) {
                                  return category.id == categoryId;
                              });

            if (it == categories.end()) {
                cout << "-> Invalid Category ID." << endl;
                continue;
            } else {
                category = it->name;
                break;
            }
        }

        displayProducts(FILE_PATH_PRODUCTS);
        if (products.empty()) {
            sleep(2);
            return;
        }

        while (true) {
            if (!JSONUtility::validateIntInput(
                    productId, "\nEnter Product ID to Update (0 = Back) : ")) {
                return;
            }

            if (productId == 0) {
                return;
            }

            for (auto &product : products) {
                if (product.id == productId) {

                    while (!JSONUtility::validateStringInput(
                        name, "\nEnter New Product Name : ")) {
                        return;
                    }

                    while (!JSONUtility::validateIntInput(
                        price, "\nEnter New Product Price : ")) {
                        return;
                    }

                    while (!JSONUtility::validateStringInput(
                        description, "\nEnter New Product Description : ")) {
                        return;
                    }

                    while (!JSONUtility::validateIntInput(
                        stock, "\nEnter New Product Stock : ")) {
                        return;
                    }

                    product.name = name;
                    product.price = price;
                    product.description = description;
                    product.category = category;
                    product.stock = stock;

                    Product::saveProducts(products, filePath);
                    cout << "\nProduct update successfully." << endl;
                    sleep(2);
                    return;
                } else {
                    cout << "Product ID not found." << endl;
                    continue;
                }
            }
        }
    }

    static void deleteProduct(const string &filePath) {
        vector<Product> products = loadProducts();
        vector<Category> categories = loadCategory();
        int productId, categoryId;

        system("cls");
        cout << "> Delete Product <" << endl;
        displayCategories(FILE_PATH_CATEGORIES);

        if (categories.empty()) {
            sleep(2);
            return;
        }

        while (true) {
            if (!JSONUtility::validateIntInput(
                    categoryId, "\nEnter Category ID (0 = Back): ")) {
                return;
            }

            if (categoryId == 0) {
                return;
            }

            auto it = find_if(categories.begin(), categories.end(),
                              [categoryId](const Category &category) {
                                  return category.id == categoryId;
                              });

            if (it == categories.end()) {
                cout << "-> Invalid Category ID." << endl;
                continue;
            } else {
                break;
            }
        }

        displayProducts(FILE_PATH_PRODUCTS);

        if (products.empty()) {
            sleep(2);
            return;
        }

        while (true) {
            if (!JSONUtility::validateIntInput(
                    productId, "\nEnter Product ID to delete (0 = Back): ")) {
                return;
            }

            if (productId == 0) {
                return;
            }

            auto it = find_if(products.begin(), products.end(),
                              [productId](const Product &product) {
                                  return product.id == productId;
                              });

            if (it == products.end()) {
                cout << "Product ID not found." << endl;
                sleep(2);
                continue;
            }

            products.erase(it);

            for (int i = 0; i < products.size(); i++) {
                products[i].id = i + 1;
            }

            Product::saveProducts(products, filePath);
            cout << "\nProduct deleted successfully." << endl;
            sleep(2);
            return;
        }
    }

    static void addCategory(vector<Category> &categories,
                            const string &filePath) {
        int id = categories.empty() ? 1 : categories.back().id + 1;
        string name;

        system("cls");
        cout << "> Add Category <" << endl;
        displayCategories(FILE_PATH_CATEGORIES);

        cin.ignore(1000, '\n');

        while (!JSONUtility::validateStringInput(
            name, "\nEnter Category Name (0 = Cancel) : ")) {
            return;
        }

        if (name == "0") {
            return;
        }

        categories.emplace_back(id, name);
        Category::saveCategories(categories, filePath);
        cout << "\nCategory added successfully." << endl;
        sleep(2);
        return;
    }

    static void editCategory(vector<Category> &categories,
                             const string &filePath) {
        int id = categories.empty() ? 1 : categories.back().id + 1;
        int categoryId;

        system("cls");
        cout << "> Edit Category <" << endl;
        displayCategories(FILE_PATH_CATEGORIES);

        if (categories.empty()) {
            sleep(2);
            return;
        }

        while (true) {

            while (!JSONUtility::validateIntInput(
                id, "\nEnter Category ID to edit (0 = Back): ")) {
                return;
            }

            if (id == 0) {
                return;
            }

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "\n-> Invalid category ID. Please try again." << endl;
                sleep(0.5);
                continue;
            } else {
                for (auto &category : categories) {
                    if (category.id == id) {
                        string name;
                        while (!JSONUtility::validateStringInput(
                            name, "\nEnter New Category Name : ")) {
                            return;
                        }
                        category.name = name;
                        Category::saveCategories(categories, filePath);
                        cout << "\nCategory edited successfully." << endl;
                        sleep(2);
                        return;
                    }
                }
                cout << "\n-> Category ID not found." << endl;
                continue;
            }
        }
    }

    static void deleteCategory(vector<Category> &categories,
                               const string &filePath) {
        int categoryId;
        system("cls");
        cout << "> Delete Category <" << endl;
        displayCategories(FILE_PATH_CATEGORIES);

        if (categories.empty()) {
            sleep(2);
            return;
        }

        while (true) {
            if (!JSONUtility::validateIntInput(
                    categoryId, "\nEnter Category ID to delete (0 = Back): ")) {
                return;
            }

            if (categoryId == 0) {
                return;
            }

            auto it = find_if(categories.begin(), categories.end(),
                              [categoryId](const Category &category) {
                                  return category.id == categoryId;
                              });

            if (it == categories.end()) {
                cout << "-> Invalid Category ID." << endl;
                sleep(2);
                continue;
            } else {
                categories.erase(it);

                for (int i = 0; i < categories.size(); i++) {
                    categories[i].id = i + 1;
                }

                Category::saveCategories(categories, filePath);
                cout << "\nCategory deleted successfully." << endl;
                sleep(2);
                return;
            }
        }
    }

    static void displayAdminStats() {
        vector<Product> products = loadProducts();
        vector<Category> categories = loadCategory();
        vector<Transaction> transactions = loadTransaction();

        system("cls");
        cout << "> Admin Stats <\n" << endl;
        cout << "Total Categories : " << categories.size() << endl;
        cout << "Total Products : " << products.size() << endl;
        cout << "Total Transactions : " << transactions.size() << endl;

        int totalRevenue = 0;
        for (const auto &transaction : transactions) {
            totalRevenue += transaction.totalPayment;
        }
        cout << "Total Revenue : Rp. " << totalRevenue << endl;
        cout << "\nPress any key to return the admin menu...";
        cin.ignore();
        cin.get();
    }
};

class Service {
  private:
    vector<User> users;
    vector<User> admins;
    vector<Product> products;
    vector<Category> categories;
    vector<Transaction> transactions;
    User *currentUser;

  public:
    Service() {
        users = Admin::loadUsers();
        products = Admin::loadProducts();
        categories = Admin::loadCategory();
        transactions = Admin::loadTransaction();
        admins = {User(1, 0, "admin", "admin", "admin", "0000")};
        currentUser = nullptr;
    }

    void displayMainMenu() {
        int choice = 0;
        while (true) {
            system("cls");
            cout << "Welcome to J-STORE" << endl;
            cout << "Your trusted platform for all you needs!\n" << endl;
            cout << "Total Users : " << users.size() << endl;
            cout << "\n1. Sign In" << endl;
            cout << "2. Sign Up" << endl;
            cout << "3. List Service" << endl;
            cout << "4. Exit" << endl;
            cout << "Choose an option: ", cin >> choice;

            if (cin.fail() || choice < 1 || choice > 4) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "-> Invalid option. Please try again.\n" << endl;
                sleep(2);
            } else {
                switch (choice) {
                    {
                    case 1:
                        signIn();
                        break;
                    case 2:
                        signUp();
                        break;
                    case 3:
                        // listService();
                        // TODO : Implement list service
                        cout << "List Service" << endl;
                        break;
                    case 4:
                        cout << "\nExiting program..." << endl;
                        system("pause");
                        return;
                    default:
                        cout << "-> Invalid option. Please try again." << endl;
                        break;
                    }
                }
            }
        }
    }

    void displayUserMenu() { displayMenu(false); }

    void displayAdminMenu() { displayMenu(true); }

    void displayMenu(bool isAdmin) {
        int choice = 0;

        while (true) {
            system("cls");

            if (isAdmin) {
                cout << "> Admin Panel - J-STORE <" << endl;
                cout << "\n1. Manage Category" << endl;
                cout << "2. Manage Product" << endl;
                cout << "3. Display Stats" << endl;
                cout << "4. Sign Out" << endl;
            } else {
                cout << "Hello, " << currentUser->fullName << "!" << endl;
                cout << "Welcome to J-STORE" << endl;
                cout << "\n> Total Users Active : " << users.size() << endl;
                cout << "> Total Transactions : " << transactions.size()
                     << endl;
                cout << "\nYour Balance : "
                     << JSONUtility::displayCurrency(currentUser->balance)
                     << endl;
                cout << "\n1. Top Up Balance" << endl;
                cout << "2. Order Product" << endl;
                cout << "3. Edit Profile" << endl;
                cout << "4. Sign Out" << endl;
            }

            cout << "Choose an option: ", cin >> choice;

            if (cin.fail() || choice < 1 || choice > 4) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "-> Invalid option. Please try again.\n" << endl;
                sleep(2);
            } else {
                if (isAdmin) {
                    switch (choice) { // Admin
                    case 1:
                        manageCategory();
                        break;
                    case 2:
                        manageProduct();
                        break;
                    case 3:
                        Admin::displayAdminStats();
                        break;
                    case 4:
                        currentUser = nullptr;
                        cout << "\nLogout successful. Wait for a moment..."
                             << endl;
                        sleep(2);
                        return;
                    default:
                        cout << "-> Invalid option. Please try again." << endl;
                        break;
                    }
                } else { // User
                    switch (choice) {
                    case 1:
                        topUpBalance();
                        break;
                    case 2:
                        orderProduct();
                        break;
                    case 3:
                        editProfile();
                        break;
                    case 4:
                        currentUser = nullptr;
                        cout << "\nLogout successful. Wait for a moment..."
                             << endl;
                        sleep(2);
                        return;
                    default:
                        cout << "-> Invalid option. Please try again." << endl;
                        break;
                    }
                }
            }
        }
    }

    void manageCategory() {
        int choice = 0;
        while (true) {
            system("cls");
            cout << "> Manage Category <" << endl;
            Admin::displayCategories(FILE_PATH_CATEGORIES);
            cout << "======================" << endl;

            cout << "\n1. Add Category" << endl;
            cout << "2. Edit Category" << endl;
            cout << "3. Delete Category" << endl;
            cout << "4. Back" << endl;
            cout << "Choose an option: ", cin >> choice;

            if (cin.fail() || choice < 1 || choice > 4) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "-> Invalid option. Please try again.\n" << endl;
                sleep(2);
            } else {
                switch (choice) {
                case 1:
                    Admin::addCategory(categories, "data/categories.json");
                    break;
                case 2:
                    Admin::editCategory(categories, "data/categories.json");
                    break;
                case 3:
                    Admin::deleteCategory(categories, "data/categories.json");
                    break;
                case 4:
                    return;
                }
            }
        }
    }

    void manageProduct() {
        int choice = 0;
        while (true) {
            system("cls");
            cout << "> Manage Product <" << endl;
            Admin::displayProducts(FILE_PATH_PRODUCTS);
            cout << "======================" << endl;
            cout << "\n1. Add Product" << endl;
            cout << "2. Edit Product" << endl;
            cout << "3. Delete Product" << endl;
            cout << "4. Back" << endl;
            cout << "Choose an option: ", cin >> choice;

            if (cin.fail() || choice < 1 || choice > 4) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "-> Invalid option. Please try again.\n" << endl;
                sleep(2);
            } else {
                switch (choice) {
                case 1:
                    Admin::addProduct(FILE_PATH_PRODUCTS);
                    break;
                case 2:
                    Admin::editProduct(FILE_PATH_PRODUCTS);
                    break;
                case 3:
                    Admin::deleteProduct(FILE_PATH_PRODUCTS);
                    break;
                case 4:
                    return;
                }
            }
        }
    }

    void topUpBalance() {
        system("cls");
        cout << "> Top Up Balance <" << endl;
        int amount, choice;

        cout << "1. Rp. 50.000" << endl;
        cout << "2. Rp. 100.000" << endl;
        cout << "3. Rp. 500.000" << endl;
        cout << "4. Custom amount" << endl;
        cout << "5. Back to menu" << endl;

        while (true) {
            cout << "Choose an option: ", cin >> choice;

            if (choice == 5) {
                return;
            }

            switch (choice) {
            case 1:
                currentUser->balance += 50000;
                break;
            case 2:
                currentUser->balance += 100000;
                break;
            case 3:
                currentUser->balance += 500000;
                break;
            case 4:
                while (!JSONUtility::validateIntInput(
                    amount, "\nEnter amount to top up: ")) {
                    return;
                }
                currentUser->balance += amount;
                break;
            default:
                cout << "Invalid option. Please try again.";
                break;
            }

            User::saveUsers(users, "data/users.json");
            cout << "\nTop Up successful. Your new balance is: "
                 << JSONUtility::displayCurrency(currentUser->balance);
            sleep(2);
            break;
        }
    }

    void orderProduct() {
        int choice = 0;
        int categoryId, productId;
        string category, quantity;

    ORDER_PRODUCT:
        vector<Category> categories = Admin::loadCategory();
        vector<Product> products = Admin::loadProducts();
        vector<Transaction> transactions = Admin::loadTransaction();

        system("cls");
        cout << "> Order Product <" << endl;
        Admin::displayCategories(FILE_PATH_CATEGORIES);

        if (categories.empty()) {
            sleep(2);
            return;
        }

        while (true) {
            if (!JSONUtility::validateIntInput(
                    categoryId, "\nChoose your category (0 = Back): ")) {
                return;
            }

            if (categoryId == 0) {
                return;
            }

            auto it = find_if(categories.begin(), categories.end(),
                              [categoryId](const Category &category) {
                                  return category.id == categoryId;
                              });

            if (it == categories.end()) {
                cout << "-> Invalid Category ID." << endl;
                sleep(0.5);
                continue;
            } else {
                category = it->name;
                break;
            }
        }

    PRODUCT_LIST:
        Admin::displayProductsByCategory(products, category);

        while (true) {
            if (!JSONUtility::validateIntInput(
                    productId, "\nChoose your product (0 = Back): ")) {
                return;
            }

            if (productId == 0) {
                goto ORDER_PRODUCT;
            }

            auto productIt =
                find_if(products.begin(), products.end(),
                        [productId, &category](const Product &product) {
                            return productId == product.id &&
                                   category == product.category;
                        });

            while (true) {
                if (!JSONUtility::validateStringInput(quantity,
                                                      "\nEnter Quantity: ")) {
                    return;
                }

                if (stoi(quantity) <= productIt->stock) {
                    break;
                } else {
                    cout << "Insufficient stock. Available : "
                         << productIt->stock << endl;
                    continue;
                }
            }

            if (productIt != products.end()) {
                system("cls");
                cout << "> Product Information <\n" << endl;
                cout << productIt->name << endl;
                cout << "> Description : " << productIt->description << endl;
                cout << "> Stock : " << productIt->stock << endl;
                cout << "> Quantity : " << quantity << endl;
                cout << "> Unit Price : "
                     << JSONUtility::displayCurrency(productIt->price) << endl;
                cout << "Total Payment : "
                     << JSONUtility::displayCurrency(stoi(quantity) *
                                                     productIt->price)
                     << endl;
                while (true) {
                    while (!JSONUtility::validateIntInput(
                        choice, "\nType '1' to Order or '0' to Cancel: ")) {
                        return;
                    }

                    if (choice == 1) {
                        cout << "Your Balance : "
                             << JSONUtility::displayCurrency(
                                    currentUser->balance)
                             << endl;
                        cout << "\nProcessing your order..." << endl;
                        sleep(2);

                        if (currentUser->balance >
                            productIt->price * stoi(quantity)) {
                            currentUser->balance -=
                                (productIt->price * stoi(quantity));
                            productIt->stock -= stoi(quantity);
                            int newTransactionId =
                                transactions.empty()
                                    ? 1
                                    : transactions.back().transactionId + 1;
                            transactions.emplace_back(
                                newTransactionId, currentUser->id,
                                productIt->id, productIt->name, stoi(quantity),
                                (productIt->price * stoi(quantity)));
                            Transaction::saveTransactions(
                                transactions, FILE_PATH_TRANSACTIONS);

                            cout << "\nPurchase successful!" << endl;
                            cout << "New Balance : " << currentUser->balance
                                 << endl;
                            cout << "\nThank you for order!\n" << endl;
                            cout << "Returning to main menu..." << endl;
                            sleep(2);
                            return;
                        } else {
                            cout << "\nInsufficient balance!." << endl;
                        TRY_AGAIN:
                            while (!JSONUtility::validateIntInput(
                                choice, "\nWould you like to TopUp? (Type '1' "
                                        "for Yes, '0' for No): ")) {
                                return;
                            }

                            if (choice == 1) {
                                cout << "Redirecting to topup menu..." << endl;
                                sleep(2);
                                topUpBalance();
                                return;
                            } else if (choice == 0) {
                                cout << "Redirecting to main menu..." << endl;
                                sleep(2);
                                return;
                            } else {
                                cout << "Invalid input. Please try again."
                                     << endl;
                                goto TRY_AGAIN;
                            }
                        }
                    } else if (choice == 0) {
                        cout << "Order cancelled. Returning to main menu....\n";
                        sleep(2);
                        return;
                    } else {
                        cout << ">Invalid input. Please try again." << endl;
                        continue;
                    }
                }

                // TODO Trx Logic
            } else {
                cout << "Product ID not found." << endl;
                continue;
            }
        }
    }

    void editProfile() {} // TODO Update Profile

    void signIn() {
        system("cls");
        string email, password;
        cout << "> Sign In to J-STORE <" << endl;
        cin.ignore(1000, '\n');
        while (true) {
            if (!JSONUtility::validateStringInput(email, "\nEnter Email: ")) {
                return;
            }

            if (!JSONUtility::validateStringInput(password,
                                                  "\nEnter Password: ")) {
                return;
            }

            for (auto &admin : admins) {
                if (admin.email == email && admin.password == password) {
                    currentUser = &admin;
                    cout << "\nSign In successful. Wait for a moment..."
                         << endl;
                    sleep(2);
                    displayAdminMenu();
                    return;
                }
            }

            for (auto &user : users) {
                if (user.email == email && user.password == password) {
                    currentUser = &user;
                    cout << "\nSign In successful. Wait for a moment..."
                         << endl;
                    sleep(2);
                    displayUserMenu();
                    return;
                }
            }
            cout << "\nInvalid email or password. Please try again." << endl;
            sleep(2);
        }
    }

    void signUp() {
        system("cls");
        cout << "> Sign Up to J-STORE <\n" << endl;
        string fullName, email, password, confirmPassword, pin;
        cin.ignore(1000, '\n');

        while (true) {
            if (!JSONUtility::validateStringInput(fullName,
                                                  "\nEnter Full Name: ")) {
                return;
            }

            if (!JSONUtility::validateStringInput(email,
                                                  "\nEnter Email Address: ")) {
                return;
            }

            while (true) {
                if (!JSONUtility::validateStringInput(password,
                                                      "\nEnter Password: ")) {
                    return;
                }

                if (!JSONUtility::validateStringInput(
                        confirmPassword, "\nEnter Confirm Password: ")) {
                    return;
                }

                if (password != confirmPassword) {
                    cout << "\nPassword do not match. Please try again."
                         << endl;
                    sleep(2);
                    continue;
                } else {
                    break;
                }
            }

            if (!JSONUtility::validateStringInput(pin,
                                                  "\nEnter PIN (4 digits): ")) {
                return;
            }

            int newId = users.empty() ? 1 : users.back().id + 1;
            users.emplace_back(newId, 0, fullName, email, password, pin);
            currentUser = &users.back();
            User::saveUsers(users, "data/users.json");
            cout << "\nSign Up successful. You can now sign in." << endl;
            sleep(2);
            displayUserMenu();
            return;
        }
    }
};
int main() {
    Service service;
    service.displayMainMenu();
    return 0;
}