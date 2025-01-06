#include "repositories/BaseRepository.hpp"

BaseRepository::BaseRepository(const string &path) : filePath(path) {
    if (!filesystem::exists(path)) {
        writeJSON(json::array());
    }
}

json BaseRepository::readJSON() {
    // lock_guard<mutex> lock(jsonMutex);
    ifstream file(filePath);
    json data = json::array();
    if (file.is_open()) {
        file >> data;
        file.close();
    }
    return data;
}

void BaseRepository::writeJSON(const json &data) {
    // lock_guard<mutex> lock(jsonMutex);
    ofstream file(filePath);
    if (file.is_open()) {
        file << data.dump(4);
        file.close();
    }
}