#include "entities/User.hpp"

User::User(int id, string fullName, string email, string password, string pin)
    : id(id), totalTransactions(0), fullName(fullName), email(email),
      password(password), pin(pin), isActive(false), isAdmin(false) {}