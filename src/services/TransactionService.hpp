#pragma once
#include "../entities/Transaction.hpp"
#include "../repositories/ProductRepository.hpp"
#include "../repositories/TransactionRepository.hpp"
#include "../repositories/UserRepository.hpp"

class TransactionService {
  private:
    TransactionRepository &transactionRepository;
    ProductRepository &productRepository;
    UserRepository &userRepository;

  public:
    TransactionService(TransactionRepository &transactionRepository,
                       ProductRepository &productRepository,
                       UserRepository &userRepository)
        : transactionRepository(transactionRepository),
          productRepository(productRepository), userRepository(userRepository) {
    }

    bool createTransaction(int userId, int productid, int quantity) {
        auto product = productRepository.findById(productid);
        auto user = userRepository.findById(userId);

        if (!product || !user)
            return false;
        if (!product->canPurchase(quantity))
            return false;

        int totalPrice = product->price * quantity;
        if (user->balance < totalPrice)
            return false;

        auto transactions = transactionRepository.getTransactions();
        int newId = transactions.empty() ? 1 : transactions.back().id + 1;

        Transaction newTransaction(newId, userId, productid, quantity,
                                   totalPrice, user->fullName, product->name);

        transactionRepository.saveTransactions(newTransaction);

        user->balance -= totalPrice;
        userRepository.saveUsers(*user);

        product->updateSold(quantity);
        productRepository.updateProduct(*product);

        return true;
    }

    vector<Transaction> getUserTransactions(int userId) {
        return transactionRepository.getUserTransaction(userId);
    }

    vector<Transaction> getAllTransactions() {
        return transactionRepository.getTransactions();
    }

    int getTotalTransactions() {
        return transactionRepository.getTransactions().size();
    }

    double getTotalRevenue() {
        auto transactions = transactionRepository.getTransactions();
        return accumulate(transactions.begin(), transactions.end(), 0.0,
                          [](double sum, const Transaction &t) {
                              return sum + t.totalPrice;
                          });
    }

    bool validateTransaction(int userId, int productId, int quantity) {
        auto product = productRepository.findById(productId);
        auto user = userRepository.findById(userId);

        if (!product || !user)
            return false;
        if (!product->canPurchase(quantity))
            return false;
        if (user->balance < product->price * quantity)
            return false;

        return true;
    }
};