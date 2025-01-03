#pragma once
#include "../entities/Transaction.hpp"
#include "BaseRepository.hpp"
#include "vector"

class TransactionRepository : public BaseRepository {
  public:
    TransactionRepository(const string &path) : BaseRepository(path) {}

    vector<Transaction> getTransactions() {
        auto data = readJSON();
        vector<Transaction> transactions;
        for (const auto &item : data) {
            transactions.emplace_back(
                item["id"], item["userId"], item["productId"], item["quantity"],
                item["totalPrice"], item["buyerName"], item["productName"]);
            transactions.back().createdAt = item["createdAt"];
            transactions.back().isCompleted = item["isCompleted"];
        }
        return transactions;
    }

    void saveTransactions(const Transaction &transaction) {
        auto data = readJSON();
        json newTransaction = {{"id", transaction.id},
                               {"userId", transaction.userId},
                               {"productId", transaction.productId},
                               {"quantity", transaction.quantity},
                               {"totalPrice", transaction.totalPrice},
                               {"buyerName", transaction.buyerName},
                               {"productName", transaction.productName},
                               {"createdAt", transaction.createdAt},
                               {"isCompleted", transaction.isCompleted}};
        data.push_back(newTransaction);
        writeJSON(data);
    }

    bool updateTransaction(const Transaction &transaction) {
        auto data = readJSON();
        for (auto &item : data) {
            if (item["id"] == transaction.id) {
                item = {{"id", transaction.id},
                        {"userId", transaction.userId},
                        {"productId", transaction.productId},
                        {"quantity", transaction.quantity},
                        {"totalPrice", transaction.totalPrice},
                        {"buyerName", transaction.buyerName},
                        {"productName", transaction.productName},
                        {"createdAt", transaction.createdAt}};
                writeJSON(data);
                return true;
            }
        }
        return false;
    }

    Transaction *findById(int id) {
        auto transaction = getTransactions();
        auto transactionIt =
            find_if(transaction.begin(), transaction.end(),
                    [id](const Transaction &t) { return t.id == id; });
        return transactionIt != transaction.end() ? &(*transactionIt) : nullptr;
    }

    vector<Transaction> getUserTransaction(int userId) {
        auto transactions = getTransactions();
        vector<Transaction> userTrans;

        copy_if(transactions.begin(), transactions.end(),
                back_inserter(userTrans),
                [userId](const Transaction &t) { return t.userId == userId; });
        return userTrans;
    }
};
