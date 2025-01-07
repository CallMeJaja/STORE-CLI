#include "services/ShoppingService.hpp"

ShoppingService::ShoppingService(ProductRepository &pRepo,
                                 UserRepository &uRepo,
                                 TransactionService &tService,
                                 CategoryRepository &cRepo)
    : productRepository(pRepo), userRepository(uRepo),
      transactionService(tService), categoryRepository(cRepo) {}

vector<Product> ShoppingService::getProductsByCategory(const string &category) {
    return productRepository.getProductsByCategory(category);
}

vector<Category> ShoppingService::getCategories() {
    return categoryRepository.getActiveCategories();
}

shared_ptr<Product> ShoppingService::getProductDetails(int productId) {
    return productRepository.findById(productId);
}

vector<Transaction> ShoppingService::getUserTransactionHistory(int userId) {
    return transactionService.getUserTransactions(userId);
}

bool ShoppingService::purchaseProduct(int userId, int productId, int quantity) {
    return transactionService.createTransaction(userId, productId, quantity);
}

bool ShoppingService::isProductAvailabe(int productId, int quantity) {
    auto product = productRepository.findById(productId);
    if (!product)
        return false;
    return product->canPurchase(quantity);
}
