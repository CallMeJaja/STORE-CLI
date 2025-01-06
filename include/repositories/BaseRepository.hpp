#pragma once
#include "fstream"
#include "json.hpp"
// #include "mutex"
#include "string"

using json = nlohmann::json;
using namespace std;
// mutex jsonMutex;

// FIXME Protect JSON access with a mutex to prevent race conditions when
// multiple threads access the file concurrently.

class BaseRepository {
  protected:
    string filePath;
    json readJSON();
    void writeJSON(const json &data);

  public:
    BaseRepository(const string &path);
};