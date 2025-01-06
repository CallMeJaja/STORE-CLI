#include "repositories/UserRepository.hpp"

UserRepository::UserRepository(const string &path) : BaseRepository(path) {}

void UserRepository::saveUsers(const User &user) {
    try {
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
    } catch (...) {
        cout << "[Error]: Failed to save user data." << endl;
    }
}

bool UserRepository::updateUser(const User &user) {
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
    return false;
}

vector<User> UserRepository::getUsers() {
    auto data = readJSON();
    vector<User> users;
    for (const auto &item : data) {
        try {
            users.emplace_back(item["id"], item["totalTransaction"],
                               item["fullName"], item["email"],
                               item["password"], item["pin"], item["balance"],
                               item["isActive"], item["isAdmin"]);
        } catch (...) {
            cout << "[Error]: Failed to parse user data." << endl;
        }
    }
    return users;
}

shared_ptr<User> UserRepository::findByEmail(const string &email) {
    auto users = getUsers();
    auto userIt =
        find_if(users.begin(), users.end(),
                [&email](const User &user) { return user.email == email; });
    if (userIt != users.end()) {
        return make_shared<User>(*userIt);
    }
    return nullptr;
}

shared_ptr<User> UserRepository::findById(int id) {
    auto users = getUsers();
    auto userIt = find_if(users.begin(), users.end(),
                          [id](const User &user) { return user.id == id; });
    if (userIt != users.end()) {
        return make_shared<User>(*userIt);
    }
    return nullptr;
}
