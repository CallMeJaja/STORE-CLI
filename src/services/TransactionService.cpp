#include "services/TransactionService.hpp"

TransactionService::TransactionService(
    TransactionRepository &transactionRepository,
    ProductRepository &productRepository, UserRepository &userRepository)
    : transactionRepository(transactionRepository),
      productRepository(productRepository), userRepository(userRepository) {}

bool TransactionService::createTransaction(int userId, int productId,
                                           int quantity) {
    if (!validateTransaction(userId, productId, quantity)) {
        return false;
    }

    auto product = productRepository.findById(productId);
    auto user = userRepository.findById(userId);

    auto transactions = transactionRepository.getTransactions();
    int newId = transactions.empty() ? 1 : transactions.back().id + 1;

    int totalPrice = product->price * quantity;
    Transaction newTransaction(newId, userId, productId, quantity, totalPrice,
                               user->fullName, product->name);

    product->updateSold(quantity);
    user->balance -= totalPrice;
    user->totalTransactions += 1;

    if (!productRepository.updateProduct(*product) ||
        !userRepository.updateUser(*user) ||
        !transactionRepository.saveTransactions(newTransaction)) {
        return false;
    }

    return true;
}

vector<Transaction> TransactionService::getUserTransactions(int userId) {
    return transactionRepository.getUserTransaction(userId);
}

vector<Transaction> TransactionService::getAllTransactions() {
    return transactionRepository.getTransactions();
}

int TransactionService::getTotalTransactions() {
    return transactionRepository.getTransactions().size();
}

double TransactionService::getTotalRevenue() {
    auto transactions = transactionRepository.getTransactions();
    return accumulate(
        transactions.begin(), transactions.end(), 0.0,
        [](double sum, const Transaction &t) { return sum + t.totalPrice; });
}

bool TransactionService::validateTransaction(int userId, int productId,
                                             int quantity) {
    auto product = productRepository.findById(productId);
    auto user = userRepository.findById(userId);

    if (!product || !user || !product->canPurchase(quantity) ||
        !user->isActive || product->price <= 0 || quantity <= 0) {
        return false;
    }

    return true;
}
