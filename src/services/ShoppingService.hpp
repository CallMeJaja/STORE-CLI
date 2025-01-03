#pragma once
#include "../repositories/CategoryRepository.hpp"
#include "../repositories/ProductRepository.hpp"
#include "../repositories/UserRepository.hpp"
#include "TransactionService.hpp"
#include "map"

class ShoppingService {
  private:
    ProductRepository &productRepository;
    UserRepository &userRepository;
    TransactionService &transactionService;
    CategoryRepository &categoryRepository;

  public:
    ShoppingService(ProductRepository &pRepo, UserRepository &uRepo,
                    TransactionService &tService, CategoryRepository &cRepo)
        : productRepository(pRepo), userRepository(uRepo),
          transactionService(tService), categoryRepository(cRepo) {}

    vector<Product> getProductsByCategory(const string &category) {
        return productRepository.getProductsByCategory(category);
    }

    vector<Category> getCategories() {
        return categoryRepository.getCategories();
    }

    Product *getProductDetails(int productId) {
        return productRepository.findById(productId);
    }

    vector<Transaction> getUserTransactionHistory(int userId) {
        return transactionService.getUserTransactions(userId);
    }

    bool purchaseProduct(int userId, int productId, int quantity) {
        return transactionService.createTransaction(userId, productId,
                                                    quantity);
    }

    bool isProductAvailabe(int productId, int quantity) {
        auto product = productRepository.findById(productId);
        return product && product->canPurchase(quantity);
    }
};