#include "entities/User.hpp"

User::User(int id, int totalTransactions, string fullName, string email,
           string password, string pin, int balance, bool isActive,
           bool isAdmin)
    : id(id), totalTransactions(totalTransactions), fullName(fullName),
      email(email), password(password), pin(pin), balance(balance),
      isActive(isActive), isAdmin(isAdmin) {}

// User::User(int id, string fullName, string email, string password, string
// pin,
//            bool isActive, bool isAdmin, int totalTransactions, int balance)
//     : id(id), fullName(fullName), email(email), password(password), pin(pin),
//       totalTransactions(totalTransactions), balance(balance),
//       isActive(isActive), isAdmin(isAdmin) {}