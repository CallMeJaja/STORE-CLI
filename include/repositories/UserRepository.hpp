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

    vector<User> getUsers();
    shared_ptr<User> findByEmail(const string &email);
    shared_ptr<User> findById(int id);
};