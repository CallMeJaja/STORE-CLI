#pragma once
#include "../../nlohmann/json.hpp"
#include "fstream"
// #include "mutex" // FIXME: Uncomment this line if you're using mutex
#include "string"

using json = nlohmann::json;
using namespace std;
// mutex jsonMutex;

class BaseRepository {
  protected:
    string filePath;

    json readJSON() {
        // lock_guard<mutex> lock(jsonMutex);
        ifstream file(filePath);
        json data;
        if (file.is_open()) {
            file >> data;
            file.close();
        }
        return data;
    }

    void writeJSON(const json &data) {
        // lock_guard<mutex> lock(jsonMutex);
        ofstream file(filePath);
        if (file.is_open()) {
            file << data.dump(4);
            file.close();
        }
    }

  public:
    BaseRepository(const string &path) : filePath(path) {}
};