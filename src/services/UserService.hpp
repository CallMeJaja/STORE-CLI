#pragma once
#include "../repositories/TransactionRepository.hpp"
#include "../repositories/UserRepository.hpp"
#include "vector"

class UserService {
  private:
    UserRepository &userRepository;
    TransactionRepository &transactionRepository;

  public:
    UserService(UserRepository &userRepository,
                TransactionRepository &transactionRepository)
        : userRepository(userRepository),
          transactionRepository(transactionRepository) {}

    bool topUpBalance(int userId, int amount) {
        if (amount <= 0)
            return false;
        auto user = userRepository.findById(userId);
        if (!user)
            return false;

        user->balance += amount;
        return userRepository.updateUser(*user);
    }

    bool toggleUserStatus(int userId) {
        auto user = userRepository.findById(userId);
        if (!user)
            return false;

        user->isActive = !user->isActive;
        return userRepository.updateUser(*user);
    }

    int getTotalUsers() { return userRepository.getUsers().size(); }

    vector<User> getActiveUsers() {
        auto users = userRepository.getUsers();
        vector<User> active;
        copy_if(users.begin(), users.end(), back_inserter(active),
                [](const User &u) { return u.isActive; });
        return active;
    }

    vector<User> getUsers() { return userRepository.getUsers(); }

    bool updateUser(User &user) { return userRepository.updateUser(user); }

    bool updateUser(int userId, const string &fullName, const string &email,
                    const string &pin) {
        auto user = userRepository.findById(userId);
        if (!user)
            return false;

        user->fullName = fullName;
        user->email = email;
        user->pin = pin;
        return updateUser(*user);
    }

    bool changePassword(int userId, const string &oldPassword,
                        const string &newPassword) {
        auto user = userRepository.findById(userId);
        if (!user || user->password != oldPassword)
            return false;

        user->password = newPassword;
        return userRepository.updateUser(*user);
    }

    double getUserTotalSpent(int userId) {
        auto transactions = transactionRepository.getTransactions();
        return accumulate(transactions.begin(), transactions.end(), 0.0,
                          [](double sum, const Transaction &t) {
                              return sum + t.totalPrice;
                          });
    }

    int getUserTotalTransactions(int userId) {
        auto transactions = transactionRepository.getTransactions();
        return transactionRepository.getUserTransaction(userId).size();
    }

    bool validateUserEmail(const string &email) {
        return !userRepository.findByEmail(email);
    }

    User *findById(int userId) { return userRepository.findById(userId); }
};