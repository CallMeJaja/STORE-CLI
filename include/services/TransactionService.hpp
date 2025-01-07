#pragma once
#include "entities/Transaction.hpp"
#include "repositories/ProductRepository.hpp"
#include "repositories/TransactionRepository.hpp"
#include "repositories/UserRepository.hpp"

class TransactionService {
  private:
    TransactionRepository &transactionRepository;
    ProductRepository &productRepository;
    UserRepository &userRepository;

  public:
    TransactionService(TransactionRepository &transactionRepository,
                       ProductRepository &productRepository,
                       UserRepository &userRepository);

    bool createTransaction(int userId, int productid, int quantity);

    vector<Transaction> getUserTransactions(int userId);
    vector<Transaction> getAllTransactions();

    int getTotalTransactions();
    double getTotalRevenue();

    bool validateTransaction(int userId, int productId, int quantity);
};