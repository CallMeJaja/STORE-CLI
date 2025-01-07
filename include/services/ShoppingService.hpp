#pragma once
#include "TransactionService.hpp"
#include "map"
#include "memory"
#include "repositories/CategoryRepository.hpp"
#include "repositories/ProductRepository.hpp"
#include "repositories/UserRepository.hpp"

class ShoppingService {
  private:
    ProductRepository &productRepository;
    UserRepository &userRepository;
    TransactionService &transactionService;
    CategoryRepository &categoryRepository;

  public:
    ShoppingService(ProductRepository &pRepo, UserRepository &uRepo,
                    TransactionService &tService, CategoryRepository &cRepo);

    vector<Product> getProductsByCategory(const string &category);
    vector<Category> getCategories();
    shared_ptr<Product> getProductDetails(int productId);
    vector<Transaction> getUserTransactionHistory(int userId);

    bool purchaseProduct(int userId, int productId, int quantity);
    bool isProductAvailabe(int productId, int quantity);
};