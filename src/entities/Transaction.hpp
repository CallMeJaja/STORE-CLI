#pragma once
#include "ctime"
#include "string"
#include "vector"
#include <algorithm>

using namespace std;

class Transaction {
  public:
    int id;
    int userId;
    int productId;
    int quantity;
    int totalPrice;
    time_t createdAt;
    string buyerName;
    string productName;
    bool isCompleted;

    Transaction(int id, int userId, int productId, int quantit, int totalPrice,
                string &buyerName, string &productName)
        : id(id), userId(userId), productId(productId), quantity(quantity),
          totalPrice(totalPrice), buyerName(buyerName),
          productName(productName) {
        createdAt = time(nullptr);
        isCompleted = false;
    }
};