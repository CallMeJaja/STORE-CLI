#pragma once
#include "../../nlohmann/json.hpp"
#include "../entities/User.hpp"
#include "BaseRepository.hpp"
#include "fstream"
#include "iostream"
#include "vector"

using json = nlohmann::json;

class UserRepository : public BaseRepository {
  private:
    bool isValidPath() {
        ifstream file(filePath);
        return file.good();
    }

  public:
    UserRepository(const string &path) : BaseRepository(path) {}

    void saveUsers(const User &user) {
        auto data = readJSON();
        json newUser = {
            {"id", user.id},
            {"totalTransaction", user.totalTransactions},
            {"fullName", user.fullName},
            {"email", user.email},
            {"password", user.password},
            {"pin", user.pin},
            {"balance", user.balance},
            {"isActive", user.isActive},
            {"isAdmin", user.isAdmin},
        };
        data.push_back(newUser);
        writeJSON(data);
    }

    bool updateUser(const User &user) {
        auto data = readJSON();

        cout << "[Debug] Updating user ID: " << user.id << endl;

        auto it = find_if(data.begin(), data.end(), [&user](const json &item) {
            return item["id"].get<int>() == user.id;
        });

        if (it != data.end()) {
            *it = {{"id", user.id},
                   {"totalTransaction", user.totalTransactions},
                   {"fullName", user.fullName},
                   {"email", user.email},
                   {"password", user.password},
                   {"pin", user.pin},
                   {"balance", user.balance},
                   {"isActive", user.isActive},
                   {"isAdmin", user.isAdmin}};
            writeJSON(data);
            return true;
        }

        cout << "[Error] User not found with ID: " << user.id << endl;
        return false;
    }

    vector<User> getUsers() {
        auto data = readJSON();

        vector<User> users;
        for (const auto &item : data) {

            users.emplace_back(item["id"], item["fullName"], item["email"],
                               item["password"], item["pin"]);
            users.back().balance = item["balance"];
            users.back().totalTransactions = item["totalTransaction"];
            users.back().isActive = item["isActive"];
            users.back().isAdmin = item["isAdmin"];
        }

        return users;
    }

    shared_ptr<User> findByEmail(const string &email) {
        auto users = getUsers();
        auto userIt =
            find_if(users.begin(), users.end(),
                    [&email](const User &user) { return user.email == email; });
        if (userIt != users.end()) {
            return make_shared<User>(*userIt);
        }

        return nullptr;
    }

    shared_ptr<User> findById(int id) {
        auto users = getUsers();
        auto userIt = find_if(users.begin(), users.end(),
                              [id](const User &u) { return u.id == id; });
        if (userIt != users.end()) {
            return make_shared<User>(*userIt);
        }
        return nullptr;
    }
};