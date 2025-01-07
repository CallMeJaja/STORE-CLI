#include "entities/Transaction.hpp"

Transaction::Transaction(int id, int userId, int productId, int quantity,
                         int totalPrice, const string &buyerName,
                         const string &productName, time_t createdAt)
    : id(id), userId(userId), productId(productId), quantity(quantity),
      totalPrice(totalPrice), createdAt(createdAt), buyerName(buyerName),
      productName(productName) {}
