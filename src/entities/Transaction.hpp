#ifndef USER_REPOSITORY_HPP
#define USER_REPOSITORY_HPP
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

    Transaction(int id, int userId, int productId, int quantity, int totalPrice,
                const string &buyerName, const string &productName)
        : id(id), userId(userId), productId(productId), quantity(quantity),
          totalPrice(totalPrice), buyerName(buyerName),
          productName(productName) {
        createdAt = time(nullptr);
    }
};

#endif