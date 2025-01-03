#pragma once
#include "../entities/User.hpp"
#include "../repositories/UserRepository.hpp"
#include "fstream"
#include "vector"

using namespace std;
using json = nlohmann::json;

class AuthService {
  private:
    UserRepository userRepository;
    vector<User> users;
    User *currentUser;
    string filePath;

  public:
    AuthService() : userRepository("data/users.json"), currentUser(nullptr) {}

    json readJSON() {
        ifstream file(filePath);
        json data;
        if (file.is_open()) {
            file >> data;
            file.close();
        }
        return data;
    }

    bool isAuthenticated() { return currentUser != nullptr; }

    bool isAdmin() { return currentUser && currentUser->isAdmin; }

    void signOut() { currentUser = nullptr; }

    bool signIn(const string &email, const string &password,
                const string &pin) {
        auto userIt = userRepository.findByEmail(email);
        if (userIt && userIt->password == password && userIt->pin == pin) {
            currentUser = userIt;
            return true;
        }
        return false;
    }

    bool signUp(const string fullName, const string &email,
                const string &password, const string &pin) {
        if (userRepository.findByEmail(email)) {
            return false;
        }
        auto usersIt = userRepository.getUsers();
        int newId = usersIt.empty() ? 1 : usersIt.back().id + 1;

        User newUser(newId, fullName, email, password, pin);
        userRepository.saveUsers(newUser);
        return true;
    }

    User *getCurrentUser() { return currentUser; }
};