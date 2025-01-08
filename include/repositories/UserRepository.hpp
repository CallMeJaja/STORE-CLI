#pragma once
#include "BaseRepository.hpp"
#include "entities/User.hpp"
#include "fstream"
#include "iostream"
#include "nlohmann/json.hpp"
#include "vector"

using json = nlohmann::json;

class UserRepository : public BaseRepository {
  private:
    bool isValidPath() {
        ifstream file(filePath);
        return file.good();
    }

  public:
    UserRepository(const string &path);

    void saveUsers(const User &user);
    bool updateUser(const User &user);

    /* {
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
     }*/

    vector<User> getUsers();
    shared_ptr<User> findByEmail(const string &email);
    shared_ptr<User> findById(int id);
};