#pragma once
#include "AuthenticationService.hpp"
#include "algorithm"
#include "map"
#include "memory"
#include "numeric"
#include "repositories/TransactionRepository.hpp"
#include "repositories/UserRepository.hpp"
#include "vector"

class UserService {
  private:
    UserRepository &userRepository;
    TransactionRepository &transactionRepository;
    shared_ptr<User> currentUser;

  public:
    // Constructor
    UserService(UserRepository &userRepository,
                TransactionRepository &transactionRepository);

    // Authentication and Authorization
    shared_ptr<User> getCurrentUser();
    void setCurrentUser(int userId);
    void clearCurrentUser();
    bool isUserActive(int userId);

    // User Management
    shared_ptr<User> findById(int userId);
    shared_ptr<User> findByEmail(const string &email);
    vector<User> getUsers();
    vector<User> getActiveUsers();
    int getTotalUsers();
    bool updateUser(User &user);
    bool toggleUserStatus(int userId);
    void updateCurrentUser(const User &user);
    void handleInactiveUser();

    // Profile Management
    bool updateProfile(int userId, const string &fullName, const string &email);
    bool changePassword(int userId, const string &newPassword,
                        const string &oldPassword);
    bool changePin(int userId, const string &newPin);

    // Balance Operation
    bool topUpBalance(int userId, int amount);

    // Transaction History
    map<string, int> getUserStats(int userId);
    vector<Transaction> getUserTransactions(int userId);
    double getUserTotalSpent(int userId);
    int getUserTotalTransactions(int userId);
    int getTotalTransactions();
};