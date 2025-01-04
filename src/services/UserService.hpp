#pragma once
#include "../repositories/TransactionRepository.hpp"
#include "../repositories/UserRepository.hpp"
#include "AuthenticationService.hpp"
#include "algorithm"
#include "map"
#include "memory"
#include "numeric"
#include "vector"

class UserService {
  private:
    UserRepository &userRepository;
    TransactionRepository &transactionRepository;
    shared_ptr<User> currentUser;

  public:
    // Constructor
    UserService(UserRepository &userRepository,
                TransactionRepository &transactionRepository)
        : userRepository(userRepository),
          transactionRepository(transactionRepository) {}

    // User Management
    shared_ptr<User> getCurrentUser() { return currentUser; }

    shared_ptr<User> findById(int userId) {
        return userRepository.findById(userId);
    }

    shared_ptr<User> findByEmail(const string &email) {
        return userRepository.findByEmail(email);
    }

    vector<User> getUsers() { return userRepository.getUsers(); }

    vector<User> getActiveUsers() {
        auto users = getUsers();
        vector<User> active;
        copy_if(users.begin(), users.end(), back_inserter(active),
                [](const User &u) { return u.isActive; });
        return active;
    }

    int getTotalUsers() { return userRepository.getUsers().size(); }

    void setCurrentUser(int userId) {
        currentUser = userRepository.findById(userId);
    }

    void clearCurrentUser() { currentUser.reset(); }

    bool updateUser(User &user) { return userRepository.updateUser(user); }

    bool toggleUserStatus(int userId) {
        auto user = userRepository.findById(userId);
        if (!user)
            return false;

        user->isActive = !user->isActive;
        return userRepository.updateUser(*user);
    }

    // Profile Management
    bool updateProfile(int userId, const string &fullName,
                       const string &email) {
        auto user = findById(userId);
        if (!user)
            return false;

        user->fullName = fullName;
        user->email = email;
        return updateUser(*user);
    }

    bool changePassword(int userId, const string &newPassword,
                        const string &oldPassword) {
        auto user = findById(userId);

        if (!user || user->password != oldPassword) {
            return false;
        }

        user->password = newPassword;
        return updateUser(*user);
    }

    bool changePin(int userId, const string &newPin) {
        auto user = findById(userId);
        if (!user)
            return false;

        user->pin = newPin;
        return updateUser(*user);
    }

    // Balance Operation
    bool topUpBalance(int userId, int amount) {
        auto user = findById(userId);
        if (!user)
            return false;

        user->balance += amount;
        if (userRepository.updateUser(*user)) {
            if (currentUser && currentUser->id == userId) {
                currentUser = user;
            }
            return true;
        }
        return false;
    }

    // Transaction History
    map<string, int> getUserStats(int userId) {
        return {
            {"totalTransactions", getUserTotalTransactions(userId)},
            {"totalSpent", static_cast<int>(getUserTotalSpent(userId))},
            {"balance", currentUser ? currentUser->balance : 0},
        };
    }

    vector<Transaction> getUserTransactions(int userId) {
        auto transactions = transactionRepository.getTransactions();
        vector<Transaction> userTransactions;

        copy_if(transactions.begin(), transactions.end(),
                back_inserter(userTransactions),
                [userId](const Transaction &t) { return t.userId == userId; });

        sort(userTransactions.begin(), userTransactions.end(),
             [](const Transaction &a, const Transaction &b) {
                 return a.createdAt > b.createdAt;
             });

        return userTransactions;
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

    int getTotalTransactions() {
        return transactionRepository.getTransactions().size();
    }

    bool validateUserEmail(const string &email) {
        return !userRepository.findByEmail(email);
    }
};