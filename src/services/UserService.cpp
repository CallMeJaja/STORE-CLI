#include "services/UserService.hpp"
#include "algorithm"
#include "numeric"

UserService::UserService(UserRepository &userRepository,
                         TransactionRepository &transactionRepository)
    : userRepository(userRepository),
      transactionRepository(transactionRepository) {}

shared_ptr<User> UserService::getCurrentUser() { return currentUser; }

void UserService::setCurrentUser(int userId) {
    currentUser = userRepository.findById(userId);
}

void UserService::clearCurrentUser() { currentUser.reset(); }

void UserService::handleInactiveUser() {
    cout << "[!]: Your account has been deactivated for some "
            "reason. "
            "Please contact admin."
         << endl;
    clearCurrentUser();
}

bool UserService::isUserActive(int userId) {
    auto user = userRepository.findById(userId);
    if (!user || !user->isActive) {
        handleInactiveUser();
        return false;
    }
    return true;
}

shared_ptr<User> UserService::findById(int userId) {
    return userRepository.findById(userId);
}

shared_ptr<User> UserService::findByEmail(const string &email) {
    return userRepository.findByEmail(email);
}

vector<User> UserService::getUsers() { return userRepository.getUsers(); }

vector<User> UserService::getActiveUsers() {
    auto users = getUsers();
    vector<User> active;
    copy_if(users.begin(), users.end(), back_inserter(active),
            [](const User &u) { return u.isActive; });
    return active;
}

int UserService::getTotalUsers() { return userRepository.getUsers().size(); }

bool UserService::updateUser(User &user) {
    return userRepository.updateUser(user);
}

bool UserService::toggleUserStatus(int userId) {
    auto user = userRepository.findById(userId);
    if (!user)
        return false;

    user->isActive = !user->isActive;

    if (updateUser(*user)) {
        setCurrentUser(user->id);
        return true;
    }

    return false;
}

bool UserService::updateProfile(int userId, const string &fullName,
                                const string &email) {
    auto user = findById(userId);
    if (!user)
        return false;

    user->fullName = fullName;
    user->email = email;
    updateCurrentUser(*user);
    return updateUser(*user);
}

bool UserService::changePassword(int userId, const string &newPassword,
                                 const string &oldPassword) {
    auto user = findById(userId);

    if (!user || user->password != oldPassword) {
        return false;
    }

    user->password = newPassword;
    updateCurrentUser(*user);
    return updateUser(*user);
}

bool UserService::changePin(int userId, const string &newPin) {
    auto user = findById(userId);
    if (!user)
        return false;

    user->pin = newPin;
    updateCurrentUser(*user);
    return updateUser(*user);
}

void UserService::updateCurrentUser(const User &updateUser) {
    if (currentUser && currentUser->id == updateUser.id) {
        currentUser = make_shared<User>(updateUser);
    }
}

bool UserService::topUpBalance(int userId, int amount) {

    auto user = findById(userId);
    if (!user)
        return false;

    user->balance += amount;
    if (userRepository.updateUser(*user)) {
        updateCurrentUser(*user);
        return true;
    }
    return false;
}

map<string, int> UserService::getUserStats(int userId) {
    return {
        {"totalTransactions", getUserTotalTransactions(userId)},
        {"totalSpent", static_cast<int>(getUserTotalSpent(userId))},
        {"balance", currentUser ? currentUser->balance : 0},
    };
}

vector<Transaction> UserService::getUserTransactions(int userId) {
    return transactionRepository.getUserTransaction(userId);
}

double UserService::getUserTotalSpent(int userId) {
    auto transactions = transactionRepository.getUserTransaction(userId);
    return accumulate(
        transactions.begin(), transactions.end(), 0.0,
        [](double sum, const Transaction &t) { return sum + t.totalPrice; });
}

int UserService::getUserTotalTransactions(int userId) {
    return transactionRepository.getUserTransaction(userId).size();
}

int UserService::getTotalTransactions() {
    return transactionRepository.getTransactions().size();
}