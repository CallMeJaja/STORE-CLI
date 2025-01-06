#include "entities/Transaction.hpp"

Transaction::Transaction(int id, int userId, int productId, int quantity,
                         int totalPrice, const string &buyerName,
                         const string &productName)
    : id(id), userId(userId), productId(productId), quantity(quantity),
      totalPrice(totalPrice), buyerName(buyerName), productName(productName) {
    createdAt = time(nullptr);
}