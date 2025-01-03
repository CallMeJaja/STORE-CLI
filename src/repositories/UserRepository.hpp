#pragma once
#include "../../nlohmann/json.hpp"
#include "../entities/User.hpp"
#include "BaseRepository.hpp"
#include "fstream"
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
        for (auto &item : data) {
            if (item["id"] == user.id) {
                item = {
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
                writeJSON(data);
                return true;
            }
        }
    }

    User *findByEmail(const string &email) {
        auto users = getUsers();
        auto userIt =
            find_if(users.begin(), users.end(),
                    [&email](const User &user) { return user.email == email; });
        if (userIt != users.end()) {
            return &(*userIt);
        }

        return nullptr;
    }

    User *findById(int id) {
        auto user = getUsers();
        auto userIt = find_if(user.begin(), user.end(),
                              [id](const User &u) { return u.id == id; });
        return userIt != user.end() ? &(*userIt) : nullptr;
    }
};