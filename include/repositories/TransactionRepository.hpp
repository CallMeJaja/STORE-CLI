#pragma once
#include "BaseRepository.hpp"
#include "entities/Transaction.hpp"
#include "iostream"
#include "memory"
#include "vector"

class TransactionRepository : public BaseRepository {
  public:
    TransactionRepository(const string &path);

    bool saveTransactions(const Transaction &transaction);
    bool updateTransaction(const Transaction &transaction);

    shared_ptr<Transaction> findById(int id);
    vector<Transaction> getTransactions();
    vector<Transaction> getUserTransaction(int userId);
};
