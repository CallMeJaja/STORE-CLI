#include "repositories/TransactionRepository.hpp"

TransactionRepository::TransactionRepository(const string &path)
    : BaseRepository(path) {}

void TransactionRepository::saveTransactions(const Transaction &transaction) {
    try {
        auto data = readJSON();
        json newTransaction = {
            {"id", transaction.id},
            {"userId", transaction.userId},
            {"productId", transaction.productId},
            {"quantity", transaction.quantity},
            {"totalPrice", transaction.totalPrice},
            {"buyerName", transaction.buyerName},
            {"productName", transaction.productName},
            {"createdAt", transaction.createdAt},
        };
        data.push_back(newTransaction);
        writeJSON(data);
    } catch (...) {
        std::cout << "[Error]: Failed to save transaction data." << endl;
    }
}

bool TransactionRepository::updateTransaction(const Transaction &transaction) {
    auto data = readJSON();
    for (auto &item : data) {
        if (item["id"] == transaction.id) {
            item = {
                {"id", transaction.id},
                {"userId", transaction.userId},
                {"productId", transaction.productId},
                {"quantity", transaction.quantity},
                {"totalPrice", transaction.totalPrice},
                {"buyerName", transaction.buyerName},
                {"productName", transaction.productName},
                {"createdAt", transaction.createdAt},
            };
            writeJSON(data);
            return true;
        }
    }
    return false;
}

shared_ptr<Transaction> TransactionRepository::findById(int id) {
    auto transactions = getTransactions();
    auto transactionIt = find_if(
        transactions.begin(), transactions.end(),
        [id](const Transaction &transaction) { return transaction.id == id; });
    if (transactionIt != transactions.end()) {
        return make_shared<Transaction>(*transactionIt);
    }
    return nullptr;
}

vector<Transaction> TransactionRepository::getTransactions() {
    auto data = readJSON();
    vector<Transaction> transactions;
    for (const auto &item : data) {
        try {
            transactions.emplace_back(item["id"], item["userId"],
                                      item["productId"], item["quantity"],
                                      item["totalPrice"], item["buyerName"],
                                      item["productName"], item["createdAt"]);
        } catch (...) {
            std::cout << "[Error]: Failed to parse transaction data." << endl;
        }
    }
    return transactions;
}

vector<Transaction> TransactionRepository::getUserTransaction(int userId) {
    auto transactions = getTransactions();
    vector<Transaction> userTransactions;

    copy_if(transactions.begin(), transactions.end(),
            back_inserter(userTransactions),
            [&userId](const Transaction &transaction) {
                return transaction.userId == userId;
            });

    return userTransactions;
}
