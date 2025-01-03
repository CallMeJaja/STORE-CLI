#pragma once
#include "string"

using namespace std;

class Category {
  public:
    int id;
    string name;
    bool isActive;

    Category(int id, string name, bool isActive = true)
        : id(id), name(name), isActive(isActive) {}
};