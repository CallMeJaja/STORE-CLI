#pragma once
#include "string"

using namespace std;

class Category {
  public:
    int id;
    string name;
    bool isActive;
    bool isDefault;

    Category(int id, string name, bool isActive = true, bool isDefault = false);
};